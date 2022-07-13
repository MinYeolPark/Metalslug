#include "Kessie.h"

//test 
#include "GameMgr.h"
#include "Menu.h"
#include "Loading.h"
//////
#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "EnemyMgr.h"
#include "EffectMgr.h"
#include "BulletMgr.h"
#include "AnimationMgr.h"
ImageInfo imgKessieInfo[];
static iImage** _imgKessie = NULL;
Kessie::Kessie(int index) : ProcEnemy(index)
{	
	layer = LayerKessie;	
	index = IdxKessie;

	state = KessieIdle;
	baseState = KessieBase100;
	headState = KessieHead;

	memset(craterState, 0x00, sizeof(craterState));
	fanState = 0;
	ai = ProcEnemyAI::KessieAI;
	memset(imgBase, 0x00, sizeof(imgBase));
	memset(imgLeftCrater, 0x00, sizeof(imgLeftCrater));
	memset(imgRightCrater, 0x00, sizeof(imgRightCrater));
	memset(imgLeftFan, 0x00, sizeof(imgLeftFan));
	memset(imgRightFan, 0x00, sizeof(imgRightFan));
	imgHead = 0;
	memset(imgLeftBlast, 0x00, sizeof(imgLeftBlast));
	memset(imgRightBlast, 0x00, sizeof(imgRightBlast));
#if 1
	colNum = 3;
	for (int i = 0; i < colNum; i++)
	{
		iSize s[3] = {
			{80, 80},{80,50},{80,50}
		};
		colliders[i]->init(this, s[i]);		
	}
	colliders[0]->isTrigger = true;
	for (int j = 0; j < 2; j++)
	{
		attkCollider[j] = new Collider();
		attkCollider[j]->enable();
		attkCollider[j]->isTrigger = true;		
	}
	
#endif

	isActive = false;
	isAppear = false;
	isHeadOpen = false;
	moveSpeed = 50.f;
	rageDt = 0.f;
	_rageDt = 5.0f;
	aiDt = 0.f;
	_aiDt = 2.f;
	_hp = 3000.f;
	hp = _hp;
	hpLeft = 2000.f;
	hpRight = 2000.f;

	if (_imgKessie == NULL)
		_imgKessie = createSingleImage(imgKessieInfo, KessieBehaveMax, this);

	for (int i = 0; i < 8; i++)
		imgBase[i] = _imgKessie[i];
	imgLeftCrater[0] = _imgKessie[8]->clone();
	imgLeftCrater[1] = _imgKessie[9]->clone();
	imgLeftCrater[2] = _imgKessie[10]->clone();
	imgRightCrater[0] = _imgKessie[11]->clone();	
	imgRightCrater[1] = _imgKessie[12]->clone();
	imgRightCrater[2] = _imgKessie[13]->clone();
	imgLeftFan[0] = _imgKessie[14]->clone();
	imgLeftFan[1] = _imgKessie[15]->clone();
	imgRightFan[0] = _imgKessie[16]->clone();
	imgRightFan[1] = _imgKessie[17]->clone();
	imgLeftBlast[0] = _imgKessie[18]->clone();
	imgRightBlast[0] = _imgKessie[18]->clone();
	imgRightBlast[0]->reverse = REVERSE_WIDTH;
	imgLeftBlast[1] = _imgKessie[19]->clone();
	imgRightBlast[1] = _imgKessie[19]->clone();
	imgRightBlast[1]->reverse = REVERSE_WIDTH;
	imgLeftBlast[2] = _imgKessie[20]->clone();
	imgRightBlast[2] = _imgKessie[20]->clone();
	imgRightBlast[2]->reverse = REVERSE_WIDTH;
	imgLeftBlast[3] = _imgKessie[21]->clone();
	imgRightBlast[3] = _imgKessie[21]->clone();
	imgRightBlast[3]->reverse = REVERSE_WIDTH;
	imgHead = _imgKessie[22]->clone();
	imgHead->stopAnimation();
}

Kessie::~Kessie()
{
	for (int i = 0; i < 13; i++)
		delete _imgKessie[i];
	delete _imgKessie;
	
	_imgKessie = NULL;
}

static iPoint initPos = iPointZero;
void Kessie::update(float dt)
{	
	if (!isAppear)
	{
		if (movePoint(p, p, iPointMake(p.x, 90), moveSpeed * 0.01))
			isAppear = true;
	}
	else//isAppear
	{
		aiDt += dt;
		if (aiDt > _aiDt)
		{
			aiDt -= _aiDt;
			addProcEnemy(IdxArMelee, iPointMake(p.x, p.y - 100), iPointZero);			
		}

		if (!isHeadOpen)
		{
			imgHead->startAnimation();
			isHeadOpen = true;
		}

		if (rageDt)
		{
			rageDt += dt;
			if (rageDt > _rageDt)
			{
				rageDt -= _rageDt;
				for (int i = 0; i < 2; i++)
				{
					blastState[i] = 3;
					attkCollider[i]->disable();
				}
			}			
		}
	}

	if (isDead)
	{		
		int maxY = *(map->maxY + (int)p.x);
		if (initPos == iPointZero)
		{
			initPos = p;
			int r = (rand() % 2) - 0.5 * 2;
			v.x = r;
		}
		int maxX;		
		if (v.x > 0)
		{
			p.x += v.x * 100 * dt;
			maxX = initPos.x + 5;
			if (p.x > maxX)
				v.x = -1;
		}
		else if (v.x < 0)
		{
			p.x += v.x * 100 * dt;
			maxX = initPos.x - 5;
			if (p.x < maxX)
				v.x = 1;
		}

		if (movePoint(p, p, iPointMake(p.x, maxY), moveSpeed * 0.5 * dt))
		{			
			//Game Clear
			for (int i = 0; i < 5; i++)
			{
				int rx = p.x - 60 + (rand() % 120);
				int ry = p.y - 50 + (rand() % 100);
				addProcEffect(EffectKessieExplosion, iPointMake(rx, ry));
			}
			for (int j = 0; j < 3; j++)
				colliders[j]->isActive = false;
			for (int k = 0; k < 2; k++)
				attkCollider[k]->isActive = false;
			isActive = false;

			//Game End			
			stageClear = true;
			if (stageClear)
				setLoading(GameStateMenu, 2, freeProc, loadMenu);
		}		
	}
	ai(this, dt);

	fixedUpdate(dt);
}

void Kessie::fixedUpdate(float dt)
{	
	for (int i = 0; i < 3; i++)
	{
		iPoint pos[3] = {
			{p.x,p.y - 10},{p.x - 80,p.y - 40},{p.x + 80, p.y - 40}
		};
		colliders[i]->update(pos[i], 0, dt);		
	}	

	for (int j = 0; j < 2; j++)
	{	
		if (containPoint(player->p + map->off, attkCollider[j]->getCollider()))
		{
			if (!player->isDead)
			{
				player->getDamage(100, player->colliders[0]);
				iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
				addProcEffect(index, bp);		//bulletIndex=effectIndex
			}
		}

		iPoint atkPos[2] = {
			{p.x + map->off.x - 88, p.y + map->off.y + 100},
			{p.x + map->off.x + 88, p.y + map->off.y + 100}
		};
		attkCollider[j]->update(atkPos[j], 0, dt);
	}

	//blastState[0] : left, blastState[1] : right
	if (blastState[0] > 1)		//not idle
	{
		attkCollider[0]->setSize(iSizeMake(
			imgLeftBlast[2]->tex->width,
			imgLeftBlast[2]->tex->height));
	}
	if (blastState[1] > 1)
	{
		attkCollider[1]->setSize(iSizeMake(
			imgRightBlast[2]->tex->width,
			imgRightBlast[2]->tex->height));
	}
}

bool Kessie::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgHead->paint(dt, { p.x + off.x, p.y + off.y - 95 });
	imgBase[baseState]->paint(dt, p + off);

	if (!isDead)
	{
		if (hpLeft > 0)
		{
			imgLeftFan[fanState]->paint(dt, { p.x + off.x - 96, p.y + off.y - 48 });			
			imgLeftCrater[craterState[0]]->paint(dt, { p.x + off.x - 88, p.y + off.y - 25 });
			imgLeftBlast[blastState[0]]->paint(dt, { p.x + off.x - 88, p.y + off.y - 42 });
		}
		if (hpRight > 0)
		{
			imgRightFan[fanState]->paint(dt, { p.x + off.x + 96, p.y + off.y - 48 });
			imgRightCrater[craterState[1]]->paint(dt, { p.x + off.x + 88, p.y + off.y - 25 });
			imgRightBlast[blastState[1]]->paint(dt, { p.x + off.x + 88, p.y + off.y -42});
		}
	}



#ifdef _DEBUG	
	drawDot(p + off);	
	for (int i = 0; i < 3; i++)
	{		
		drawRect(colliders[i]->getCollider());
	}
	for (int j = 0; j < 2; j++)
		drawRect(attkCollider[j]->getCollider());
	
#endif // DEBUG
	setRGBA(1, 1, 1, 1);
	
	return !isActive;
}

void Kessie::free()
{
}

void Kessie::getDamage(float damage, Collider* c)
{
	for (int i = 0; i < colNum; i++)
	{
		if (containPoint(c->p, colliders[i]->getCollider()))
		{
			if (i == 0)
				hp -= damage;
			else if (i == 1)
			{
				hp -= damage;
				hpLeft -= damage;
			}
			else if (i == 2)
			{
				hp -= damage;
				hpRight -= damage;
			}
			printf("hp=%d, leftHp=%d, rightHp=%d\n", hp, hpLeft, hpRight);
		}
	}

	if (hp <= _hp * 0.7)
	{
		state = KessieRage;
		ai = ProcEnemyAI::KessieRageAI;
		rageDt = 0.000001f;
		for (int i = 0; i < 2; i++)
		{
			fanState = 1;
			if (blastState[i] == 0)		//if Blast_Idle
				blastState[i] = 1;
		}

		//movedirection init
		if (v == iPointZero)
		{
			int r = (rand() % 2 - 0.5) * 2;
			v.x = r;
			v.y = r;
		}
	}

	if (hp <= _hp * 0.4)
	{
		if (baseState == KessieBaseBoth60
			|| baseState == KessieBaseLeft60
			|| baseState == KessieBaseRight60)
		{			
			state = KessieBase40;
		}
	}
	else if (hp <= _hp * 0.6)
	{
		if (baseState == KessieBase80)
			state = KessieBase40;

	}
	else if (hp <= _hp * 0.8)
	{
		if (baseState == KessieBase100)
			state = KessieBase80;

	}

	if (hp <= 0 && !isDead)
		dead();
}

void Kessie::setState(int newState)
{
}

bool Kessie::dead()
{
	isDead = true;
	state = KessieBase0;	
	for (int i = 0; i < 2; i++)
		attkCollider[i]->disable();
	int rx = p.x - 60 + rand() % 120;
	int ry = p.y - 50 + rand() % 100;
	addProcEffect(EffectKessieExplosion, iPointMake(rx, ry));

	return isDead;
}


ImageInfo imgKessieInfo[] =
{
	{
		"assets/Kessie/Base_00.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},	
	{
		"assets/Kessie/Base_01.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_02.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_03.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_04.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_05.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_06.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_07.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_%02d.png",
		3, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_70_%02d.png",
		15, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_Dead_%02d.png",
		6, 1.f, { -104 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_%02d.png",
		3, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_70_%02d.png",
		15, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_Dead_%02d.png",
		6, 1.f, { -54 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanLeft_%02d.png",
		3, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanLeft_Rage_%02d.png",
		6, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanRight_%02d.png",
		3, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanRight_Rage_%02d.png",
		6, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_Idle_%02d.png",
		4, 1.f, { -48 / 2 , -48},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_%02d.png",
		25, 1.f, { -50 / 2 , -160},
		0.06f,
		1,
		{0,248,0,255},
		AnimationMgr::cbAniKessieBlast,
	},
	{
		"assets/Kessie/Blasting_%02d.png",
		6, 1.f, { -54 / 2 , -160},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_End_%02d.png",
		16, 1.f, { -54 / 2 , -160},
		0.06f,
		1,
		{0,248,0,255},
		AnimationMgr::cbAniKessieBlastEnd,
	},
	{
		"assets/Kessie/Head_%02d.png",
		7, 1.f, { -96 / 2 , 0},
		0.18f,
		1,
		{0,248,0,255},
		NULL,
	},
};
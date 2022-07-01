#include "Kessie.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "EnemyMgr.h"
#include "EffectMgr.h"
#include "BulletMgr.h"
ImageInfo imgKessieInfo[];
static iImage** _imgKessie = NULL;
Kessie::Kessie(int index) : ProcEnemy(index)
{	
	layer = LayerKessie;	
	index = IdxKessie;

	state = KessieIdle;
	baseState = KessieBase100;
	headState = KessieHead;	
	for (int i = 0; i < 2; i++)
	{
		//craterState[i] = KessieCraterLeft100 + i;
		fanState = KessieBlast;
		//blastState[i] = KessieBlastLeft + i;
	}
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
#endif

	isActive = false;
	isAppear = false;
	isHeadOpen = false;
	moveSpeed = 50.f;
	effDt = 0.f;
	_effDt = 3.0f;
	aiDt = 0.f;
	_aiDt = 2.f;
	_hp = 1000.f;
	hp = _hp;
	hpLeft = 800.f;
	hpRight = 800.f;

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
	imgHead = _imgKessie[20]->clone();
	imgHead->stopAnimation();
}

Kessie::~Kessie()
{
	for (int i = 0; i < 13; i++)
		delete _imgKessie[i];
	delete _imgKessie;
	
	_imgKessie = NULL;
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
	if (hp <= _hp * 0.4)
	{
		if(baseState==KessieBaseBoth60
			||baseState==KessieBaseLeft60
			||baseState==KessieBaseRight60)
			setState(KessieBase40);
	}
	else if (hp <= _hp * 0.6)
	{
		if(baseState==KessieBase80)
			setState(KessieBaseBoth60);
	}
	else if (hp <= _hp * 0.8)
	{
		if(baseState==KessieBase100)
			setState(KessieBase80);
	}

	if (hp <= 0)
		dead();
}

float delay = 0.f, _delay = 1.f;
void Kessie::setState(int newState)
{
	baseState = newState;
	printf("%d\n", newState);

	if (delay >= _delay)
	{
		delay -= _delay;
	}
	
}

bool Kessie::dead()
{
	isDead = true;
	ai = ProcEnemyAI::KessieDeadAI;	
	setState(KessieBase0);

	int rx = p.x - 60 + rand() % 120;
	int ry = p.y - 50 + rand() % 100;
	addProcEffect(EffectKessieExplosion, iPointMake(rx, ry));
	
	return isDead;
}

void Kessie::update(float dt)
{	
	if (!isAppear)
	{
		if (movePoint(p, p, iPointMake(p.x, p.y + 50), moveSpeed))
			isAppear = true;
	}
	else
	{
		if (!isHeadOpen)
		{
			imgHead->startAnimation();
			isHeadOpen = true;
		}

		aiDt += dt;
		if (aiDt > _aiDt)
		{
			aiDt -= _aiDt;
			addProcEnemy(IdxArMelee, iPointMake(100, 0), iPointZero);
			if (hp < _hp * 0.6 + 1)
			{
				state = KessieRage;
				for (int i = 0; i < 2; i++)
				{
					craterState;
					fanState = KessieBlastRage;
					//blastState[i] = KessieBlastRageLeft + i;
				}
				ai = ProcEnemyAI::KessieRageAI;
				//movedirection init
				if (v == iPointZero)
				{
					int r = (rand() % 2 - 0.5) * 2;
					v.x = r;
				}			
			}
		}

	}

	if (isDead)
	{
		delay += dt;
		int maxY = *(map->maxY + (int)p.x);
		if (movePoint(p, p, iPointMake(p.x, maxY), moveSpeed * 0.8 * dt))
		{
			//Game Clear
			for (int i = 0; i < 5; i++)
			{
				int rx = p.x - 60 + (rand() % 120);
				int ry = p.y - 50 + (rand() % 100);
				addProcEffect(EffectKessieExplosion, iPointMake(rx, ry));
			}
			for (int i = 0; i < 3; i++)
			{
				colliders[i]->isActive = false;
				colliders[i]->isTrigger = false;
				//remove from list
			}
			isActive = false;
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
		colliders[i]->setPosition(pos[i]);
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
			imgLeftFan[fanState%2]->paint(dt, { p.x + off.x - 96, p.y + off.y - 48 });
			//imgLeftCrater[0]->paint(dt, { p.x + off.x - 88, p.y + off.y - 25 });
			//imgLeftBlast[0]->paint(dt, { p.x + off.x - 88, p.y + off.y + imgLeftBlast[0]->tex->height - 40 });
		}
		if (hpRight > 0)
		{
			imgRightFan[fanState%2]->paint(dt, { p.x + off.x + 96, p.y + off.y - 48 });
			//imgRightCrater[0]->paint(dt, { p.x + off.x + 88, p.y + off.y - 25 });
			//imgRightBlast[0]->paint(dt, { p.x + off.x + 88, p.y + off.y + imgRightBlast[0]->tex->height - 40} );
		}
	}
#ifdef _DEBUG	
	drawDot(p + off);	
	for (int i = 0; i < 3; i++)
	{		
		drawRect(colliders[i]->getCollider());
	}
	
	//drawRect(leftCollider->getCollider());
	//drawRect(rightCollider->getCollider());
#endif // DEBUG
	setRGBA(1, 1, 1, 1);
	
	return !isActive;
}

void Kessie::free()
{
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
		4, 1.f, { -48 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_%02d.png",
		47, 1.f, { -48 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
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
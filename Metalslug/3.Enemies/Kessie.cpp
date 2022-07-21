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
	memset(imgBase, 0x00, sizeof(imgBase));
	memset(imgLeftCrater, 0x00, sizeof(imgLeftCrater));
	memset(imgRightCrater, 0x00, sizeof(imgRightCrater));
	memset(imgLeftFan, 0x00, sizeof(imgLeftFan));
	memset(imgRightFan, 0x00, sizeof(imgRightFan));
	imgHead = 0;
	memset(imgLeftBlast, 0x00, sizeof(imgLeftBlast));
	memset(imgRightBlast, 0x00, sizeof(imgRightBlast));
#if 1
	rectNum = 3;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();
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
	_hp = 500.f;
	for (int i = 0; i < 3; i++)
		hp[i] = _hp;
	//test
	hp[1] = 200;
	hp[2] = 200;

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
void Kessie::init(iPoint p)
{
	this->p = p;
	tp = p;

	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(80, 60);
		r->origin = p;
	}

	for (int i = 0; i < 2; i++)
	{
		atkRect[i] = new iRect();
		iRect* r = atkRect[i];
		r->size = iSizeMake(40, 80);
		iPoint pos[2] =
		{
			{p.x - 80, p.y + 30},
			{p.x + 80, p.y + 30},
		};
		r->origin = pos[i];
	}
}

float x = 0;
float delta = 0;
void Kessie::update(float dt)
{	
	if (isDead)
	{
		for (int i = 0; i < rectNum; i++)
			rect[i]->size = iSizeZero;
		return;
	}

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
			//addProcEnemy(IdxArMelee, iPointMake(p.x, p.y - 100), iPointZero);			
		}

		if (!isHeadOpen)
		{
			imgHead->startAnimation();
			isHeadOpen = true;
		}

		if (rageDt)
		{
			rageDt += dt;
#if 1 //sin move
			delta += dt;
			while (delta > 3)
				delta -= 3;
			float r = delta / 3;
			x = _cos(360 * r);
			p.x += x;
#endif 
			if (rageDt > _rageDt)
			{
				rageDt -= _rageDt;
				fanState = 1;
				for (int i = 0; i < 2; i++)
					blastState[i] = 3;
			}			
		}
	}	
	fixedUpdate(dt);
}

void Kessie::fixedUpdate(float dt)
{	
	//ColliderUpdate		
	iPoint rectPos[3] =
	{
		{p.x, p.y},
		{ p.x - 80, p.y - 30 },
		{ p.x + 80, p.y - 30 },
	};

	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			rectPos[i].x + map->off.x - rect[i]->size.width / 2,
			rectPos[i].y + map->off.y - rect[i]->size.height);		
	}
	 
#if 0
	for (int i = 0; i < 2; i++)
	{
		iPoint pos[2] =
		{
			{p.x - 80, p.y + 30},
			{p.x + 80, p.y + 30},
		};
		atkRect[i]->origin = iPointMake(
			pos[i].x + map->off.x - atkRect[i]->size.width / 2,
			pos[i].y + map->off.y - atkRect[i]->size.height);		
		drawRect(iRectMake(atkRect[i]->origin.x + map->off.x,
			atkRect[i]->origin.y+ map->off.y,
			atkRect[i]->size.width,
			atkRect[i]->size.height));
		printf("rectPos[%d]=%f,%f\n", i, pos[i].x, pos[i].y);
		printf("size=%f, %f\n", atkRect[i]->size.width, atkRect[i]->size.height);
	}
#endif
}

bool Kessie::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgHead->paint(dt, { p.x + off.x, p.y + off.y - 95 });
	imgBase[baseState]->paint(dt, p + off);

	if (!isDead)
	{
		if (hp[1] > 0)
		{
			imgLeftFan[fanState]->paint(dt, { p.x + off.x - 96, p.y + off.y - 48 });			
			imgLeftCrater[craterState[0]]->paint(dt, { p.x + off.x - 88, p.y + off.y - 25 });
			imgLeftBlast[blastState[0]]->paint(dt, { p.x + off.x - 88, p.y + off.y - 42 });
		}
		if (hp[2] > 0)
		{
			imgRightFan[fanState]->paint(dt, { p.x + off.x + 96, p.y + off.y - 48 });
			imgRightCrater[craterState[1]]->paint(dt, { p.x + off.x + 88, p.y + off.y - 25 });
			imgRightBlast[blastState[1]]->paint(dt, { p.x + off.x + 88, p.y + off.y -42});
		}
	}

	if (alphaDt)
	{
		alphaDt += dt;
		alpha = fabsf(_cos((alphaDt / _alphaDt * 270)));
		if (alphaDt > _alphaDt)
		{
			alphaDt = 0.0f;
			isActive = false;
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect(i));
	
	if (isDead)
	{
		int maxY = *(map->maxY + (int)p.x);	

#if 1 //sin move
		delta += dt;
		float r = delta / 0.2;
		x = 5 * _sin(360 * r);
		p.x += x;
#endif 
		if (movePoint(p, p, iPointMake(p.x, maxY), moveSpeed * 0.7 * dt))
		{
			//Game Clear
			for (int i = 0; i < 3; i++)
			{
				int rx = p.x - 60 + (rand() % 120);
				int ry = p.y - 50 + (rand() % 100);
				addProcEffect(EffectKessieExplosion, iPointMake(rx, ry));
			}
			isActive = false;

			//Game End			
			stageClear = true;
			if (stageClear)
				;// setLoading(GameStateMenu, 2, freeProc, loadMenu);
		}
	}

#endif // _DEBUG
	setRGBA(1, 1, 1, 1);
	
	return !isActive;
}

void Kessie::free()
{
}

void Kessie::getDamage(float damage)
{
	for (int i = 0; i < bulletNum; i++)
	{
		for (int k = 0; k < rectNum; k++)
		{
			ProcBullets* b = bullets[i];
			if (containPoint(b->p + map->off, rect[k][0]))
			{
				hp[k] -= damage;
				printf("hp[%d]=%d\n", k, hp[k]);
			}
		}

		//Check all destroyed
		if (hp[0] <= 0)
		{
			isDead = true;
			baseState = KessieBase0;
		}
	}

	if (hp[0] <= _hp * 0.7)
	{
		state = KessieRage;	
		rageDt = 0.000001f;
	}

	if (hp[0] <= _hp * 0.4)
	{
		if (baseState == KessieBaseBoth60
			|| baseState == KessieBaseLeft60
			|| baseState == KessieBaseRight60)
		{			
			state = KessieBase40;
		}
	}
	else if (hp[0] <= _hp * 0.6)
	{
		if (baseState == KessieBase80)
			state = KessieBase40;

	}
	else if (hp[0] <= _hp * 0.8)
	{
		if (baseState == KessieBase100)
			state = KessieBase80;

	}
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
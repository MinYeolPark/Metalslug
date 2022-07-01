#include "Mosque.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "BulletMgr.h"

//#include "ProcBullets.h"
#include "InputMgr.h"

ImageInfo imgMosqueBaseInfo[];
ImageInfo imgMosqueAddInfo[];
static iImage** _imgMosqueBase = NULL;
static iImage** _imgMosqueAdd = NULL;
Mosque::Mosque(int index) : ProcEnemy(index)
{	
	layer = LayerMosque;
	index = IdxMosque;

	state = MosqueIdle;
	memset(imgBase, 0x00, sizeof(imgBase));
	memset(imgDoor, 0x00, sizeof(imgDoor));

	memset(imgLeftTower, 0x00, sizeof(imgLeftTower));
	memset(imgMidTower, 0x00, sizeof(imgMidTower));
	memset(imgRightTower, 0x00, sizeof(imgRightTower));

	memset(imgLeftCurtain, 0x00, sizeof(imgLeftCurtain));
	memset(imgMidCurtain, 0x00, sizeof(imgMidCurtain));
	memset(imgRightCurtain, 0x00, sizeof(imgRightCurtain));

	memset(imgShutter, 0x00, sizeof(iImage*) * 6);
	memset(imgSoldier, 0x00, sizeof(iImage*) * 6);

	baseState = MosqueIdle;
	doorState = MosqueIdle;
	memset(towerState, 0x00, sizeof(towerState));
	memset(shutterState, 0x00, sizeof(shutterState));
	memset(curtainState, 0x00, sizeof(curtainState));
	memset(soldierState, 0x00, sizeof(soldierState));

	memset(doorP, 0x00, sizeof(doorP));
	memset(shutterP, 0x00, sizeof(shutterP));
	memset(isShutterOpen, false, sizeof(isShutterOpen));
	memset(curtainP, 0x00, sizeof(curtainP));
	memset(soldierP, 0x00, sizeof(soldierP));
	memset(towerP, 0x00, sizeof(towerP));
	
#if 1
	colNum = 3;
	for (int i = 0; i < colNum; i++)
	{
		iSize s[3] = {
			{60, 60},{60,60},{60,60}
		};
		colliders[i]->init(this, s[i]);
	}
#endif

	isActive = false;
	isAppear = false;
	memset(isShutterOpen, 0x00, sizeof(isShutterOpen));		
	memset(hp, 0x00, sizeof(hp));
	_hp = 0.f;

	if (_imgMosqueBase == NULL)
		_imgMosqueBase = createSingleImage(imgMosqueBaseInfo, 16, this);
	if (_imgMosqueAdd == NULL)
		_imgMosqueAdd = createSingleImage(imgMosqueAddInfo, 4, this);


	doorP[0] = { p.x - 60, p.y - 3 };
	doorP[1] = { p.x + 110, p.y - 3 };

	towerP[0] = { p.x - 60, p.y - 50 };
	towerP[1] = { p.x + 40, p.y - 50 };
	towerP[2] = { p.x + 130, p.y - 50 };
#if 1
	imgBase[MosqueIdle] = _imgMosqueBase[0]->clone();
	imgBase[MosqueDead] = _imgMosqueBase[1]->clone();
	imgDoor[MosqueIdle] = _imgMosqueBase[2]->clone();
	imgDoor[MosqueDead] = _imgMosqueBase[3]->clone();
	imgLeftTower[MosqueIdle] = _imgMosqueBase[4]->clone();
	imgLeftTower[MosqueDead] = _imgMosqueBase[5]->clone();
	imgMidTower[MosqueIdle] = _imgMosqueBase[6]->clone();
	imgMidTower[MosqueDead] = _imgMosqueBase[7]->clone();
	imgRightTower[MosqueIdle] = _imgMosqueBase[8]->clone();
	imgRightTower[MosqueDead] = _imgMosqueBase[9]->clone();
	imgLeftCurtain[MosqueIdle] = _imgMosqueBase[10]->clone();
	imgLeftCurtain[MosqueDead] = _imgMosqueBase[11]->clone();
	imgMidCurtain[MosqueIdle] = _imgMosqueBase[12]->clone();
	imgMidCurtain[MosqueDead] = _imgMosqueBase[13]->clone();
	imgRightCurtain[MosqueIdle] = _imgMosqueBase[14]->clone();
	imgRightCurtain[MosqueDead] = _imgMosqueBase[15]->clone();
#endif
	for (int i = 0; i < 3; i++)
	{
		imgShutter[i][MosqueIdle] = _imgMosqueAdd[0]->clone();
		imgShutter[i][MosqueDead] = _imgMosqueAdd[1]->clone();
		imgSoldier[i][MosqueIdle] = _imgMosqueAdd[2]->clone();
		imgSoldier[i][MosqueDead] = _imgMosqueAdd[3]->clone();
	}

	_hp = 1500.f;
	for (int i = 0; i < 3; i++)
	{
		hp[i] = 100.f;
		float t = rand() % 100;
	}
}

Mosque::~Mosque()
{
	for (int i = 0; i < MosqueBehaveMax; i++)
	{
		delete _imgMosqueBase[i];
		delete _imgMosqueAdd[i];
	}
	delete _imgMosqueBase;
	delete _imgMosqueAdd;

	_imgMosqueBase = NULL;
	_imgMosqueAdd = NULL;
}

void Mosque::init(int index, iPoint p, iPoint v)
{
	ProcEnemy::init(index, p, v);
	doorP[0] = { p.x - 60, p.y - 3 };
	doorP[1] = { p.x + 110, p.y - 3 };

	towerP[0] = { p.x - 60, p.y - 50 };
	towerP[1] = { p.x + 40, p.y - 50 };
	towerP[2] = { p.x + 130, p.y - 50 };

	for (int i = 0; i < 3; i++)
		hp[i] = _hp;
}

void Mosque::update(float dt)
{		
	for(int i = 0; i < 3; i++)
	{
		if (movePoint(towerP[i], towerP[i], 
			iPointMake(towerP[i].x, p.y - 80), 1))
			shutterState[i] = MosqueDead;

		if (isShutterOpen[i])
		{
			
		}
	}
	shutterP[0] = { towerP[0].x + 2, towerP[0].y - 70 };
	shutterP[1] = { towerP[1].x + 2, towerP[1].y - 70 };
	shutterP[2] = { towerP[2].x + 5, towerP[2].y - 70 };

	curtainP[0] = { shutterP[0].x - 5, shutterP[0].y + 5 };
	curtainP[1] = { shutterP[1].x - 5, shutterP[1].y + 5 };
	curtainP[2] = { shutterP[2].x - 5, shutterP[2].y + 5 };

	soldierP[0] = { shutterP[0].x - 5, shutterP[0].y - 5 };
	soldierP[1] = { shutterP[1].x - 5, shutterP[1].y - 5 };
	soldierP[2] = { shutterP[2].x - 5, shutterP[2].y - 5 };

	if (isDead)
	{
		for(int i=0;i<3;i++)
			addProcEffect(EffectExplosion, towerP[i]);
	}
	fixedUpdate(dt);
}
void Mosque::fixedUpdate(float dt)
{
	for (int i = 0; i < 3; i++)
		colliders[i]->update(
			iPointMake(towerP[i].x, towerP[i].y - colliders[i]->getCollider().size.height));
}
bool Mosque::dead()
{
	setState(MosqueDead);
	isDead = true;
	for (int i = 0; i < colNum; i++)
	{
		if (colliders[i]->isActive == false)
		{
			towerState[i] = MosqueDead;
		}
	}
	return isDead;
}
void Mosque::getDamage(float damage, Collider* c)
{
	for (int i = 0; i < colNum; i++)
	{
		if (containPoint(c->p, colliders[i]->getCollider()))
		{
			if(c == colliders[i])
				hp[i] -= damage;		
			printf("hp=%f\n", hp[i]);
			if (hp[i] < 1)
			{
				colliders[i]->disable();
				dead();
			}
		}
	}
}
void Mosque::setState(int newState)
{
	state = newState;
}

void Mosque::free()
{
}

bool Mosque::draw(float dt, iPoint off)
{
	imgLeftTower[towerState[0]]->paint(dt, towerP[0] + off);
	imgMidTower[towerState[1]]->paint(dt, towerP[1] + off);
	imgRightTower[towerState[2]]->paint(dt, towerP[2] + off);

	imgBase[MosqueIdle]->paint(dt, p + off);
		
	for (int i = 0; i < 3; i++)
	{
		if (towerState[i] != MosqueDead)
		{
			imgSoldier[i][soldierState[i]]->paint(dt, soldierP[i] + off);
			imgLeftCurtain[curtainState[i]]->paint(dt, curtainP[0] + off);
			imgMidCurtain[curtainState[i]]->paint(dt, curtainP[1] + off);
			imgRightCurtain[curtainState[i]]->paint(dt, curtainP[2] + off);
			//imgShutter[i][shutterState[i]]->paint(dt, shutterP[i] + off);
		}
	}
	

#ifdef _DEBUG
	drawDot(p);
	
	for (int i = 0; i < 3; i++)
	{
		iRect c = colliders[i]->getCollider();
		c.origin.x += off.x;
		c.origin.y += off.y;
		drawRect(c);
	}
#endif // _DEBUG

	return !isActive;
}

void Mosque::cbAniShutterOpen(void* parm)
{
	printf("cbAni Door Open\n");
	Mosque* m = (Mosque*)parm;
	for (int i = 0; i < 3; i++)
	{
		m->curtainState[i] = MosqueDead;
		m->isShutterOpen[i] = true;
	}
}

void Mosque::cbAniCurOpen(void* parm)
{
	printf("Curtain was opened\n");
}

void Mosque::cbAniAddOut(void* parm)
{
	Mosque* mt = (Mosque*)parm;

	for (int i = 0; i < 3; i++)
	{
		
	}
	mt->isActive = false;	
}

ImageInfo imgMosqueBaseInfo[] =
{
	{
		"assets/Mosque/MosqueBase.png",
		1, 1.f, { -370 / 2 , 0},
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueBase_Dead.png",
		1, 1.f, { -369 / 2, 0},
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Add.png",
		1, 1.f, { -38 / 2, 0},
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Mosque/MidBoss_Add_Destroyed_%02d.png",
		14, 1.f, { -151 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_00.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_00.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_01.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_01.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_02.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_02.png",
		3, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueLeftCurtain_%02d.png",
		5, 1.f, { -48 / 2, 0},
		0.18f,
		0,
		{32,8,0,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueLeftCurtain_Open_%02d.png",
		13, 1.f, { -64 / 2, 0},
		0.06f,
		1,
		{32,8,0,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueMidCurtain_%02d.png",
		6, 1.f, { -48 / 2, 0},
		0.18f,
		0,
		{32,8,0,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueMidCurtain_Open_%02d.png",
		13, 1.f, { -64 / 2, 0},
		0.06f,
		1,
		{32,8,0,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueRightCurtain_%02d.png",
		6, 1.f, { -48 / 2, 0},
		0.18f,
		0,
		{32,8,0,255},
		NULL,
		},
	{
		"assets/Mosque/MosqueRightCurtain_Open_%02d.png",
		13, 1.f, { -64 / 2, 0},
		0.06f,
		1,
		{32,8,0,255},
		NULL,
	},
};

ImageInfo imgMosqueAddInfo[] =
{
	{
		"assets/Mosque/MosqueDoor_00.png",
		1, 1.f, { -97 / 2, 0},
		0.1f,
		0,
		{63,72,204,255},
		NULL,
	},
	{
		"assets/Mosque/MosqueDoor_%02d.png",
		16, 1.f, { -97 / 2, 0},
		0.1f,
		1,
		{63,72,204,255},
		Mosque::cbAniShutterOpen,
	},
	{
		"assets/Mosque/Mosque_Soldier_%02d.png",
		8, 1.f, { -34 / 2, 0},
		0.1f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Soldier_Dead_%02d.png",
		7, 1.f, { -43 / 2, 0},
		0.1f,
		1,
		{255,0,0,255},
		NULL,
	},
};

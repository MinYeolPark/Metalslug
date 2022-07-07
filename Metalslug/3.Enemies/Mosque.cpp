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

	memset(imgTower, 0x00, sizeof(imgTower));
	memset(imgCurtain, 0x00, sizeof(imgTower));


	memset(imgShutter, 0x00, sizeof(imgShutter));
	memset(imgSoldier, 0x00, sizeof(imgSoldier));
	//memset(imgSoldier, 0x00, sizeof(iImage*) * 6);

	baseState = MosqueIdle;
	doorState = MosqueIdle;
	memset(towerState, 0x00, sizeof(towerState));
	memset(shutterState, 0x00, sizeof(shutterState));
	memset(curtainState, 0x00, sizeof(curtainState));
	memset(soldierState, 0x00, sizeof(soldierState));

	memset(doorP, 0x00, sizeof(doorP));
	memset(shutterP, 0x00, sizeof(shutterP));
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
	memset(hp, 0x00, sizeof(hp));
	_hp = 0.f;

	if (_imgMosqueBase == NULL)
		_imgMosqueBase = createSingleImage(imgMosqueBaseInfo, 12, this);
	if (_imgMosqueAdd == NULL)
		_imgMosqueAdd = createSingleImage(imgMosqueAddInfo, 4, this);
#if 1
	imgBase[0] = _imgMosqueBase[0]->clone();
	imgBase[1] = _imgMosqueBase[1]->clone();
	imgDoor[0] = _imgMosqueBase[2]->clone();
	imgDoor[1] = _imgMosqueBase[3]->clone();

#endif
	for (int i = 0; i < 3; i++)
	{
		imgTower[i][0] = _imgMosqueBase[4]->clone();
		imgTower[i][0]->stopAnimation();
		imgTower[i][1] = _imgMosqueBase[5]->clone();
		imgTower[i][1]->stopAnimation();

		imgCurtain[0][0] = _imgMosqueBase[6]->clone();
		imgCurtain[0][1] = _imgMosqueBase[7]->clone();
		imgCurtain[1][0] = _imgMosqueBase[8]->clone();
		imgCurtain[1][1] = _imgMosqueBase[9]->clone();
		imgCurtain[2][0] = _imgMosqueBase[10]->clone();
		imgCurtain[2][1] = _imgMosqueBase[11]->clone();
		

		imgShutter[i][0] = _imgMosqueAdd[0]->clone();
		imgShutter[i][1] = _imgMosqueAdd[1]->clone();
		imgSoldier[i][0] = _imgMosqueAdd[2]->clone();
		imgSoldier[i][1] = _imgMosqueAdd[3]->clone();
	}

	_hp = 1500.f;
	sight = 500.f;
	attkRange = 500.f;
	for (int i = 0; i < 3; i++)
	{
		hp[i] = 100.f;		
		firePoint[i] = iPointZero;
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

	towerP[0] = { p.x - 70, p.y - 50 };
	towerP[1] = { p.x + 30, p.y - 50 };
	towerP[2] = { p.x + 125, p.y - 50 };

	for (int i = 0; i < 3; i++)
		hp[i] = _hp;

	//fix
	fpNum = 3;
	memset(aiDt, 0x00, 3);
	aiDt[0] = 3.f;
	aiDt[1] = 5.f;
	aiDt[2] = 0.f;
	_aiDt = 6.f;		//fire prer 2 sec
}

static float dramaDt = 0.f, _dramaDt = 5.f;
static float intervalDt = 0.f, _intervalDt = 0.18f;
void Mosque::update(float dt)
{		
	//Appear
	for(int i = 0; i < 3; i++)
	{
		if (movePoint(towerP[i], towerP[i],
			iPointMake(towerP[i].x, p.y - 80), 1))
			shutterState[i] = 1;

		if (curtainState[i]==1 && towerState[i]!=MosqueDead)
		{
			aiDt[i] += dt;
			if (aiDt[i] > _aiDt)
			{
				aiDt[i] -= _aiDt;
				tp = player->p;
				iPoint vp = tp - firePoint[i];
				float ang = iPointAngle(iPointMake(1, 0), iPointZero, vp);
				int len = iPointLength(tp - p);
				if (len < attkRange)
				{					
					printf("ang=%f\n", ang);
					addBullet(this, i, BulletMosque, 360-ang);		//x축 기준, 시계방향 degree
					addProcEffect(EffectMoskTrail, firePoint[i]);
				}
			}
		}
	}
	//Update
	for (int i = 0; i < 3; i++)
	{
		shutterP[i] = { towerP[i].x + 10, towerP[i].y - 70 };
		if (i == 2)
			shutterP[i].x += 6;
		curtainP[i] = { shutterP[i].x - 5, shutterP[i].y + 5 };
		soldierP[i] = { shutterP[i].x - 5, shutterP[i].y - 5 };
		firePoint[i] = soldierP[i];
	}

	//Dead Event
	if (isDead)
	{
		dramaDt += dt;
		intervalDt += dt;
		if (intervalDt > _intervalDt)
		{
			intervalDt -= _intervalDt;
			int rx = p.x + (((rand() % 150) - 50) * 2);
			int ry = p.y - (((rand() % 50)) * 2);
			addProcEffect(EffectExplosionM, iPointMake(rx,ry));
		}

		if (dramaDt > _dramaDt)
		{
			dramaDt -= _dramaDt;
			isDead = true;
			isActive = false;
			for (int i = 0; i < 10; i++)
			{
				int rx = p.x + (rand() % 80 - 40 * 2) * i;
				int ry = p.y - (rand() % 40) * i;
				addProcEffect(EffectExplosionM, iPointMake(rx, ry));
			}
		}
	}
	fixedUpdate(dt);
}

void Mosque::fixedUpdate(float dt)
{	
	for (int i = 0; i < 3; i++)
	{
		if (colliders[i]->isActive)
		{
			colliders[i]->update(
				iPointMake(towerP[i].x, towerP[i].y - colliders[i]->getCollider().size.height),
				degree, dt);
		}
	}
}

bool Mosque::draw(float dt, iPoint off)
{
	//Sort
	for (int i = 0; i < 3; i++)
	{
		imgTower[i][towerState[i]]->paint(dt, towerP[i] + off);
		imgTower[i][towerState[i]]->frame = i;
		if (towerState[i] != MosqueDead)
		{
			imgSoldier[i][soldierState[i]]->paint(dt, soldierP[i] + off);
			imgCurtain[i][curtainState[i]]->paint(dt, curtainP[i] + off);			
			imgShutter[i][shutterState[i]]->paint(dt, shutterP[i] + off);
		}
	}
	imgBase[MosqueIdle]->paint(dt, p + off);
	//render sort



#ifdef _DEBUG
	drawDot(p);
	for (int i = 0; i < 3; i++)
	{
		if (colliders[i]->isActive)
		{
			iRect c = colliders[i]->getCollider();
			c.origin.x += off.x;
			c.origin.y += off.y;		
			drawRect(c);
		}
	}
#endif // _DEBUG

	return !isActive;
}
iPoint Mosque::getFirePoint()
{
	
	return iPoint();
}
bool Mosque::dead()
{
	for (int i = 0; i < colNum; i++)
	{
		if (colliders[i]->isActive == false)
		{
			if(towerState[i]!=MosqueDead)
				addProcEffect(EffectExplosionM, shutterP[i]);
			towerState[i] = MosqueDead;
		}
	}
	
	if (towerState[0] == MosqueDead &&
		towerState[1] == MosqueDead &&
		towerState[2] == MosqueDead)
	{
		isDead = true;
		setState(MosqueDead);
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

void Mosque::cbAniShutterOpen(void* parm)
{
	printf("cbAni Door Open\n");
	Mosque* m = (Mosque*)parm;
	for (int i = 0; i < 3; i++)
	{
		m->curtainState[i] = 1;
		m->shutterState[i] = 1;
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
		"assets/Mosque/Mosque_%02d.png",
		3, 1.f, { -64 / 2, 0},
		0.06f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_%02d.png",
		3, 1.f, { -64 / 2, 0},
		0.06f,
		1,
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

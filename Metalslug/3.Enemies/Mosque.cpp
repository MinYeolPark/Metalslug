#include "Mosque.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"

#include "ProcBullets.h"
#include "InputMgr.h"

ImageInfo imgMosqueBaseInfo[];
ImageInfo imgMosqueAddInfo[];
static iImage** _imgMosqueBase = NULL;
static iImage** _imgMosqueAdd = NULL;
Mosque::Mosque()
{	
	memset(imgBase, 0x00, sizeof(imgBase));
	memset(imgDoor, 0x00, sizeof(imgDoor));

	memset(imgLTower, 0x00, sizeof(imgLTower));
	memset(imgMTower, 0x00, sizeof(imgMTower));
	memset(imgRTower, 0x00, sizeof(imgRTower));

	memset(imgShutter, 0x00, sizeof(iImage*) * 6);
	memset(imgCurtain, 0x00, sizeof(iImage*) * 6);
	memset(imgSoldier, 0x00, sizeof(iImage*) * 6);

	memset(doorP, 0x00, sizeof(doorP));
	memset(shutterP, 0x00, sizeof(shutterP));
	memset(isShutterOpen, false, sizeof(isShutterOpen));
	memset(curtainP, 0x00, sizeof(curtainP));
	memset(soldierP, 0x00, sizeof(soldierP));
	memset(towerP, 0x00, sizeof(towerP));
	
	baseState = MosqueIdle;
	doorState = MosqueIdle;
	for (int i = 0; i < 3; i++)
	{
		towerState[i] = MosqueIdle;
	}

	p = iPointMake(230,200);
	s = iSizeZero;

	isActive = false;
		
	memset(hp, 0x00, sizeof(hp));
	_hp = 0.f;
	memset(rate, 0x00, sizeof(rate));
	_rate = 0.f;

	if (imgMosqueBaseInfo == NULL)
		_imgMosqueBase = createMosqueBaseImage(imgMosqueBaseInfo, 10, this);
	if (imgMosqueAddInfo == NULL)
		_imgMosqueAdd = createMosqueBaseImage(imgMosqueAddInfo, 2, this);
}

Mosque::~Mosque()
{
	for (int i = 0; i < MosqueBehaveMax; i++)
		delete _imgMosqueBase[i];
	delete _imgMosqueBase;

	_imgMosqueBase = NULL;
}
void Mosque::initObj()
{
	doorP[0] = { p.x - 60, p.y - 3 };
	doorP[1] = { p.x + 110, p.y - 3 };

	towerP[0] = { p.x - 50, p.y - 50 };
	towerP[1] = { p.x + 40, p.y - 50 };
	towerP[2] = { p.x + 130, p.y - 50 };

	shutterP[0] = towerP[0];
	shutterP[1] = towerP[1];
	shutterP[2] = towerP[2];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			imgBase[j] = _imgMosqueBase[j]->clone();
			imgDoor[j] = _imgMosqueBase[j + 2]->clone();
			imgLTower[j] = _imgMosqueBase[j + 4]->clone();
			imgMTower[j] = _imgMosqueBase[j + 6]->clone();
			imgRTower[j] = _imgMosqueBase[j + 8]->clone();
		}
		
	}

	for (int j = 0; j < 3; j++)
	{
		imgShutter[j][MosqueIdle] = _imgMosqueAdd[0]->clone();
		imgShutter[j][MosqueDead] = _imgMosqueAdd[1]->clone();
	}
};

void Mosque::updateObj(float dt)
{
	
#if 0
	for (int i = 0; i < 3; i++)
	{
		setDoorPos(towerP[i]);

		if (flag)
		{
			if (imgDoor[i]->frame==0)
			{
				imgCurtain[i][state]->startAnimation(cbAniCurOpen, this);
				imgDoor[i]->startAnimation(cbAniDoorOpen, this);
			}
		}
		rate[i] += dt;
		if (rate[i] > _rate)
		{
			rate[i] -= _rate;			
			fire(dt);
		}
	}
#endif
		
#if 1
	if (getKeyStat(keyboard_space))
	{
		baseState = MosqueDead;
		doorState = MosqueDead;
	}
	else
	{
		baseState = MosqueIdle;
		doorState = MosqueIdle;
	}
#endif
}
void Mosque::fixedUpdate(float dt)
{
}
void Mosque::fire(float dt)
{
}
void Mosque::dead()
{
	
}
bool Mosque::drawObj(float dt, iPoint off)
{
	for (int i = 0; i < 2; i++)
	{
		imgDoor[doorState]->paint(dt, doorP[i] + off);
	}
	
	imgLTower[towerState[0]]->paint(dt, towerP[0] + off);
	imgMTower[towerState[1]]->paint(dt, towerP[1] + off);
	imgRTower[towerState[2]]->paint(dt, towerP[2] + off);
	
	for (int j = 0; j < 3; j++)
		imgShutter[j][shutterState[j]]->paint(dt, shutterP[j] + off);
	imgBase[baseState]->paint(dt, p + off);





	#ifdef _DEBUG
		setRGBA(1, 0, 1, 1);
		drawRect(off.x + p.x - imgBase[baseState]->tex->width / 2,
			off.y + p.y - imgBase[baseState]->tex->height,
			imgBase[baseState]->tex->width,
			imgBase[baseState]->tex->height);
		drawDot(p + off);
		setRGBA(1, 1, 1, 1);
	#endif // DEBUG
	return !isActive;
}

void Mosque::freeeObj()
{
}


void Mosque::cbAniDoorOpen(void* parm)
{
	printf("cbAni Door Open\n");
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
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_00.png",
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
		{
		"assets/Mosque/Mosque_01.png",
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_01.png",
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
		{
		"assets/Mosque/Mosque_02.png",
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Mosque/Mosque_Dead_02.png",
		1, 1.f, { -86 / 2, 0},
		0.18f,
		0,
		{255,0,0,255},
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
		0,
		{63,72,204,255},
		NULL,
	},
};

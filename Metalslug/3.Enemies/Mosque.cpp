#include "Mosque.h"

#include "UIMgr.h"
#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "BulletMgr.h"

//#include "ProcBullets.h"
#include "InputMgr.h"
#include "AnimationMgr.h"
ImageInfo imgMosqueBaseInfo[];
ImageInfo imgMosqueAddInfo[];
static iImage** _imgMosqueBase = NULL;
static iImage** _imgMosqueAdd = NULL;
Mosque::Mosque(int index) : ProcEnemy(index)
{	
	score = 100;
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
	_dmgDt = 0.18f;
	for (int i = 0; i < 3; i++)
	{
		hp[i] = 100.f;		
		dmgDt[i] = 0.f;
		firePoint[i] = iPointZero;
	}
}

// d/_d * 360 =>  0 / 90 / 270 / 360 
//				  90 / 0 / 270 / 0
// 
// // 0~1~1~1
// float r = angle; while( r > 90 ) r -= 90; r /= 90;
// 
// int array[4] = {90, 0, -90, 0};// 90 / 0 / 270 / 0
// float degree = array[n] * (1-r)* + array[(1+n)%4] * r;

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

void Mosque::init(iPoint p)
{
	this->p = p;
	doorP[0] = { p.x - 60, p.y - 3 };
	doorP[1] = { p.x + 110, p.y - 3 };

	towerP[0] = { p.x - 70, p.y - 50 };
	towerP[1] = { p.x + 30, p.y - 50 };
	towerP[2] = { p.x + 125, p.y - 50 };

	for (int i = 0; i < 3; i++)
	{		
		hp[i] = _hp;
		soldierP[i] = shutterP[i];
	}

	//fix
	memset(aiDt, 0x00, 3);
	for (int i = 0; i < 3; i++)
		firePoint[i] = soldierP[i];
	aiDt[0] = 3.f;
	aiDt[1] = 5.f;
	aiDt[2] = 0.f;
	_aiDt = 6.f;		//fire prer 2 sec	

	rectNum = 3;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();

	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(60, 60);
	}
}

static float dramaDt = 0.f, _dramaDt = 5.f;
static float intervalDt = 0.f, _intervalDt = 0.3f;
void Mosque::update(float dt)
{		
	if (isDead)
		return;

	if (!isAppear)
	{
		if (containPoint(p,
			iRectMake(-map->off.x - 180, -map->off.y,
				devSize.width, devSize.height)))
		{
			isAppear = true;
			if (map->move(iPointMake(devSize.width / 5 - (p.x + map->_off.x), 0)))
				map->isClipped = true;
			audioPlay(snd_eff_mosqueMove);
		}
		return;
	}
	else
	{
		isActive= containPoint(p,
			iRectMake(-map->off.x - 200, -map->off.y - 40,
				devSize.width + 400, devSize.height + 80));
		isDead = !isActive;		
	}
	//Appear	
	for (int i = 0; i < 3; i++)
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
					//addBullet(this, BulletMosque, 0, i);		//x축 기준, 시계방향 degree 	
					addBullet(this, BulletMosque, 360-ang, i);		//x축 기준, 시계방향 degree 										
					addProcEffect(this, EffectMoskTrail, firePoint[i]);
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
		if (soldierState[i] != MosqueDead)
			soldierP[i] = { shutterP[i].x - 5, shutterP[i].y - 5 };
		firePoint[i] = soldierP[i];
	}

	fixedUpdate(dt);
}

void Mosque::fixedUpdate(float dt)
{	
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			shutterP[i].x + map->off.x - rect[i]->size.width / 2,
			shutterP[i].y + map->off.y - rect[i]->size.height);
	}
}

bool Mosque::draw(float dt, iPoint off)
{
	//Sort
	for (int i = 0; i < 3; i++)
	{
		imgTower[i][towerState[i]]->paint(dt, towerP[i] + off);
		imgTower[i][towerState[i]]->frame = i;
		//if (soldierState[i] == MosqueDead)
		//	soldierP[i].y += (50 * dt);
		if (towerState[i] != MosqueDead)
		{
			if (dmgDt[i])
			{
				setRGBA(1, 0.8, 0.1, 1);				
				dmgDt[i] += dt;
				if (dmgDt[i] > _dmgDt)
					dmgDt[i] = 0.f;
			}
			else
				setRGBA(1, 1, 1, 1);

			imgSoldier[i][soldierState[i]]->paint(dt, soldierP[i] + off);
			imgCurtain[i][curtainState[i]]->paint(dt, curtainP[i] + off);
			imgShutter[i][shutterState[i]]->paint(dt, shutterP[i] + off);
			setRGBA(1, 1, 1, 1);
		}
	}
	imgBase[baseState]->paint(dt, p + off);

	//Dead Event
	if (dramaDt)
	{
		dramaDt += dt;
		intervalDt += dt;
		if (intervalDt > _intervalDt)
		{
			audioPlay(snd_eff_expM);
			intervalDt -= _intervalDt;
			int rx = p.x + (((rand() % 150) - 50) * 2);
			int ry = p.y - (((rand() % 50)) * 2);
			addProcEffect(this, EffectExplosionM, iPointMake(rx, ry));
		}

		if (dramaDt > _dramaDt)
		{
			audioPlay(snd_eff_expL);
			state = MosqueDead;
			for (int i = 0; i < 10; i++)
			{
				int rx = p.x + (rand() % 120 - 40 * 2) * i;
				int ry = p.y - (rand() % 40) * i;
				addProcEffect(this, EffectExplosionM, iPointMake(rx, ry));
			}
			isDead = true;
			baseState = MosqueDead;
			dramaDt = 0.0f;
			map->isClipped = false;
		}
	}

	if (getKeyStat(keyboard_delete))
		#define DEBUG
#ifdef DEBUG
	for (int i = 0; i < rectNum; i++)
	{
		if (!isAppear)
			setRGBA(1, 1, 1, 0);
		drawRect(getRect(i));
	}
#endif // _DEBUG
	return !isActive;
}
iPoint Mosque::getFirePoint()
{
	
	return iPoint();
}

void Mosque::getDamage(float damage)
{
	for (int i = 0; i < bulletNum; i++)
	{
		for (int k = 0; k < rectNum; k++)
		{		
			ProcBullets* b = bullets[i];
			if (containPoint(b->p + map->off, getRect(k)))
			{
				hp[k] -= damage;
				score = 100;
				if(!dmgDt[k])
					dmgDt[k] = 0.000001f;				
				printf("#1 %f, #2 %f, #3 %f\n", rect[0]->size.width, rect[1]->size.width, rect[2]->size.width);
				if (hp[k] <= 0)
				{
					rect[k]->size = iSizeZero;		//disable
					
					towerState[k] = MosqueDead;
					soldierState[k] = MosqueDead;
					shutterState[k] = MosqueDead;
				}
			}
		}

		//Check all destroyed
		if (hp[0] <= 0 && hp[1] <= 0 && hp[2] <= 0)
		{
			isDead = true;
			dramaDt = 0.0000001f;
		}
	}	
}

void Mosque::free()
{
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
		3, 1.f, { -80 / 2, 0},
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
		AnimationMgr::cbAniShutterOpen,
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


bool canCollision(int index)
{
	// 0 ~ 99 : monstrer
	// 100 ~ 200 : npc
	if (index < 100)
		return true;
	else if (index < 200)
		return false;

	return true;
}

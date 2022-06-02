#include "ProcBullets.h"

#include "iStd.h"

#include "ImgMgr.h"

static iImage** _imgBullets = NULL;
ImageInfo bulletImageInfo[];
ProcBullets::ProcBullets(int idx) : ProcObject(idx)
{
	imgs = NULL;
	imgCurr = NULL;
	parent = NULL;
	bulletIdx = idx;
	iPoint v = iPointZero;

	pattern = NULL;
	speed = 100.;
	dmg = 100.;	

	if (_imgBullets == NULL)
		_imgBullets = createImgBullets(bulletImageInfo, this);

	imgs = new iImage * [BulletIndexMax];
	memset(imgs, 0x00, sizeof(iImage*) * BulletIndexMax);
	for (int i = 0; i < BulletIndexMax; i++)
		imgs[i] = _imgBullets[i]->clone();
}

ProcBullets::~ProcBullets()
{
	for (int i = 0; i < BulletIndexMax; i++)
	{
		delete _imgBullets[i];
		delete imgs[i];
	}
	delete _imgBullets;
	delete imgs;

	_imgBullets = NULL;
}

void ProcBullets::initObj()
{
	isActive = true;

}

void ProcBullets::initObj(ProcObject* parent, int idx, iPoint p, float degree)
{
	isActive = true;
	this->parent = parent;
	this->bulletIdx = idx;
	this->p = p;
	this->degree = degree;
	v = iPointRotate(iPointMake(1, 0), iPointZero, degree) * 300;
}

void ProcBullets::updateObj(float dt)
{
	isActive = containPoint(p, iRectMake(-20, -20, devSize.width + 40, devSize.height + 40));

	if (parent->layer == Player)
	{
		ProcEnemy* eNear = NULL;
		float dNear = 0xffffff;
		for (int j = 0; j < enemyCount; j++)
		{
			ProcEnemy* e = enemies[j];
			if (containPoint(p, e->collider()))
			{
				float d = iPointLength(p - e->p);
				if (dNear > d)
				{
					dNear = d;
					eNear = e;
				}
			}
		}

		if (eNear)
		{
			isActive = false;
			eNear->hp -= dmg;
			if (eNear->hp <= 0)
				eNear->state = ((EnemyBehave)(DeadEnemyL + eNear->state % 2));
			//ÆÄÆ¼Å¬
		}
	}
}

bool ProcBullets::drawObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[bulletIdx];
	imgCurr->paint(dt, p + off);

	p += v * dt;

	setRGBA(1, 1, 1, 1);
	return !isActive;
}

void ProcBullets::freeObj()
{
}

ImageInfo bulletImageInfo[PlayerBehaveMax] =
{
	{
		"assets/Bullets/HandGun_%02d.png",
		1,
		1.0f, {-12 / 2, -8 / 2},
		0.18f,
		1,
		{255, 255, 255 ,255},
		NULL,
	},
	{
		"assets/Bullets/HeavyMachineGun_%02d.png",
		1,
		1.0f, {-30 / 2,-5/2},
		0.18f,
		1,
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		29,
		1.0f, {-50 / 2,-26 / 2},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
};




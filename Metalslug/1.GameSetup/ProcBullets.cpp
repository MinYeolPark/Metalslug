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

}

void ProcBullets::updateObj(float dt)
{
	for (int i = 0; i < bulletNum; i++)
	{
		ProcBullets* b = bullets[i];	
		if (b->isActive == false)
		{
			bulletNum--;
			bullets[i] = bullets[bulletNum];
			i--;
		}
	}
	//Pattern Update
#if 0
#if 0
	b->p = parent->p + iPointRotate(parent->firePoint, iPointZero, 360 - d[dir]);
#else
	int d[4] = { 0, 180, 90, 360 };
	if (parent->layer == ObjLayer::Player)
	{
		ProcPlayer* own = (ProcPlayer*)parent;
		float y = own->firePoint.y;
		own->firePoint.y = 0;
		iPoint mp = iPointRotate(own->firePoint, iPointZero, 360 - d[dir]);
		mp.y += y;
		own->firePoint.y = y;
		p = parent->p + mp;
	}
	v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - d[dir]);
#endif
#endif
}



void ProcBullets::drawObj(float dt, iPoint off)
{
	if (isActive)
	{
		imgCurr = imgs[bulletIdx];
		imgCurr->paint(dt, p + off);
	}
#ifdef _DEBUG

#endif // _DEBUG

	p += v * speed * dt;
	setRGBA(1, 1, 1, 1);
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




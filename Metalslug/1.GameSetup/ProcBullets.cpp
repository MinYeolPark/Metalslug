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
		b->pattern;
		if (b->isActive == false)
		{
			bulletNum--;
			bullets[i] = bullets[bulletNum];
			i--;
		}
	}
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
		2.0f, {-12 / 2, -8 / 2},
		0.18f,
		1,
		iColor4fMake(255.,255.,255.,255),
		NULL,
	},
	{
		"assets/Bullets/HeavyMachineGun_%02d.png",
		1,
		2.0f, {-30 / 2,-5/2},
		0.18f,
		1,
		iColor4fMake(255.,255.,255.,255),
		NULL,
	},
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		29,
		2.0f, {-50 / 2,-26 / 2},
		0.1f,
		0,
		iColor4fMake(255.,255.,255.,255),
		NULL,
	},
};




#include "ProcBullets.h"

#include "ImgMgr.h"
#include "Proc.h"
#include "ProcField.h"

static iImage** _imgBullets = NULL;
ImageInfo imageInfo[];
ProcBullets::ProcBullets() : ProcObject()
{
	imgs = NULL;
	imgCurr = NULL;
	parent = NULL;
	bulletIdx = 0;
	iPoint v = iPointZero;
	dmg = 0.;

	if (_imgBullets == NULL)
		_imgBullets = createImgChar(imageInfo, this);
}

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
	printf("proc bullets!");

	if (_imgBullets == NULL)
		_imgBullets = createImgBullets(imageInfo, this);
	imgs = new iImage * [BulletIndexMax];
	memset(imgs, 0x00, sizeof(iImage*) * BulletIndexMax);
	for (int i = 0; i < BulletIndexMax; i++)
		imgs[i] = _imgBullets[i]->clone();
}

ProcBullets::~ProcBullets()
{
	for (int i = 0; i < BulletIndexMax; i++)
		delete imgs[i];
	delete imgs;
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

void ProcBullets::drawObj(float dt, iPoint p)
{
	imgCurr = imgs[bulletIdx];
	imgCurr->paint(dt, p + bg->off);

#ifdef _DEBUG
	drawRect(collider().origin.x + bg->off.x, collider().origin.y + bg->off.y,
		collider().size.width, collider().size.height);
#endif // _DEBUG

	p += v * speed * dt;
	setRGBA(1, 1, 1, 1);
}

void ProcBullets::freeObj()
{
}

ImageInfo imageInfo[] =
{
	{
		"assets/Bullets/HandGun.png",
		1,
		2.0f, {0,0},
		0.18f,
	},
	{
		"assets/Bullets/HeavyMachineGun.png",
		2,
		2.0f, {-30 / 2,-5},
		0.18f,
	},

};




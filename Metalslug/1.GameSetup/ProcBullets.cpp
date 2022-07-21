#include "ProcBullets.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "BulletMgr.h"

#include "Kessie.h"

static iImage** _imgBullets = NULL;
ImageInfo bulletImageInfo[];
ProcBullets::ProcBullets(int index) : ProcObject()
{
	layer = LayerBullet;

	imgs = NULL;
	imgCurr = NULL;

	parent = NULL;
	this->index = index;
	degree = 0;

	pow = 0.f;
	up = 0.f;
	down = 0.f;

	speed = 0;
	damage = 0;

	if (_imgBullets == NULL)
		_imgBullets = createSingleImage(bulletImageInfo, BulletIndexMax, this);
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

void ProcBullets::fixedUpdate(float dt)
{
	
}

bool ProcBullets::draw(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, alpha);
	imgCurr = imgs[index];
	imgCurr->degree = degree;
	imgCurr->paint(dt, p + off);
	setRGBA(1, 1, 1, 1);

#ifdef _DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect());
#endif // _DEBUG

	return !isActive;
}

void ProcBullets::free()
{
}

void ProcBullets::getDamage(float damage, Collider* c)
{
	printf("get damage bulletss\n");
	hp -= damage;
	if (hp <= 0)
		isActive = false;
}

ImageInfo bulletImageInfo[] =
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
		"assets/Bullets/Bomb_%02d.png",
		16,
		.5f, {-20 / 2, 0},
		0.1f,
		0,
		{0, 248, 0, 255},
		NULL,
	},
	///////////////////////////////////////////////////////
	{
		"assets/Bullets/Melee_%02d.png",
		7,
		1.0f, {-72 / 2,0},
		0.04f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		30,
		1.0f, {-50/2,0},
		0.06f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/Bullets/MosqueTrace_%02d.png",
		3,
		1.0f, {-40 / 2,0},
		0.08f,
		0,
		{255, 255, 255, 255},
		NULL,
	},


	//No Effect
	{
		"assets/Bullets/Melee_07.png",
		1,
		1.0f, {-72 / 2,0},
		0.06f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
};




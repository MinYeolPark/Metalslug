#include "ProcBullets.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "BulletMgr.h"

#include "Kessie.h"

static iImage** _imgBullets = NULL;
ImageInfo bulletImageInfo[];
ProcBullets::ProcBullets(int idx)
{
	layer = LayerBullet;
	imgs = NULL;
	imgCurr = NULL;
	parent = NULL;
	index = idx;
	p = iPointZero;
	v = iPointZero;
	a = 1.f;
	pow = 0.f;
	up = 0.f;
	down = 0.f;
	collider = new Collider();
	collider->isActive = true;	
	pattern = NULL;
	speed = 0;
	damage = 0;

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

void ProcBullets::init(ProcPlayer* parent, int index, float degree)
{
	this->isActive = true;
	this->parent = parent;
	this->index = index;	
	this->p = parent->firePoint;
	this->degree = degree;
	BulletPattern bp[BulletIndexMax] = {
	ProcBulletsPattern::patternHandgun,
	ProcBulletsPattern::patternHeavyMachinegun,
	ProcBulletsPattern::patternBomb,
	ProcBulletsPattern::patternMelee,
	ProcBulletsPattern::patternMosque,
	};
	iSize bs[BulletIndexMax] =
	{
		{10,5},
		{25, 5},
	};
	this->collider->init(parent, bs[index]);
	this->pattern = bp[index];
	this->speed = player->curGun->speed;
	this->damage = player->curGun->dmg;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);
	imgs[this->index]->startAnimation();
	if (index == BulletBomb)
	{
		pow = 5.f;
		up = 0.f;
		down = 0.0f;
		imgs[BulletMeleeEnd]->startAnimation(AnimationMgr::cbAniBulletDisappearWithAlpha, this);
	}
}

void ProcBullets::init(ProcEnemy* parent, int index, float degree)
{
	this->isActive = true;
	this->parent = parent;
	this->index = index;
	this->p = parent->fp;
	this->degree = degree;
	BulletPattern bp[BulletIndexMax] = {
		ProcBulletsPattern::patternHandgun,
		ProcBulletsPattern::patternHeavyMachinegun,
		ProcBulletsPattern::patternBomb,
		ProcBulletsPattern::patternMelee,
		ProcBulletsPattern::patternMosque,
	};
	this->pattern = bp[index];
	//adjust
	this->speed = 100;
	this->damage = 100;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);
	imgs[this->index]->startAnimation();
	if (index == BulletMelee)
	{
		pow = 5.f;
		up = 0.f;
		down = 0.0f;
		imgs[BulletMeleeEnd]->startAnimation(AnimationMgr::cbAniBulletDisappearWithAlpha, this);
	}
}

void ProcBullets::init(ProcObject* parent, int index, float degree)
{
	this->isActive = true;
	this->parent = parent;
	this->index = index;
	this->p = parent->p;
	this->degree = degree;
	BulletPattern bp[BulletIndexMax] = {
			ProcBulletsPattern::patternHandgun,
			ProcBulletsPattern::patternHeavyMachinegun,
			ProcBulletsPattern::patternBomb,
			ProcBulletsPattern::patternMelee,
			ProcBulletsPattern::patternMosque,
	};
	this->pattern = bp[index];	
	this->speed = player->curGun->speed;
	this->damage = player->curGun->dmg;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);
	imgs[this->index]->startAnimation();
}

void ProcBullets::update(float dt)
{	
#if 1
	isActive = containPoint(p,
	iRectMake(-map->off.x - 20, -map->off.y - 20,
		devSize.width + 40, devSize.height + 40));
#endif
	pattern(this, dt);
	
	fixedUpdate(dt);
}

void ProcBullets::fixedUpdate(float dt)
{
	if (parent->layer == LayerPlayer)
	{
		Collider* cNear = NULL;
		float dNear = 0xffffff;
		for (int i = 0; i < objects->count; i++)
		{
			Collider* c = (Collider*)objects->objectAtIndex(i);
			if (c->parent->layer != LayerPlayer)
			{
				if (containPoint(p, c->getCollider()))
				{
					float d = iPointLength(p - c->p);
					if (dNear > d)
					{
						if (c->isActive)
						{
							dNear = d;
							cNear = c;
						}
					}
				}
				if (cNear)
				{
					if (cNear->isActive)
					{
						isActive = false;
						cNear->parent->getDamage(damage, cNear);
						iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
						addProcEffect(index, bp);		//bulletIndex=effectIndex
					}
				}
			}
			cNear = NULL;
		}
	}
	else
	{
		{
			for (int i = 0; i < player->colNum; i++)
			{
				if (containPoint(p, player->colliders[i]->getCollider()))
				{
					if (!player->isDead)
					{
						isActive = false;
						player->getDamage(damage, player->colliders[i]);
						iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
						addProcEffect(index, bp);		//bulletIndex=effectIndex
					}
				}
			}
		}
	}
	
	//update
	//collider->update();
	
	collider->setPosition(p);
	if (degree == 90 || degree == 270)
		collider->setSize(
			iSizeMake(collider->getCollider().size.height, collider->getCollider().size.width));
	else
		collider->setSize(
			iSizeMake(collider->getCollider().size.width, collider->getCollider().size.height));
}

bool ProcBullets::draw(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, a);
	imgCurr = imgs[index];
	imgCurr->degree = degree;			
	imgCurr->paint(dt, p + off);
#ifdef _DEBUG				
	setDotSize(5);
#endif // DEBUG
	
	setRGBA(1, 1, 1, 1);
	return !isActive;
}

void ProcBullets::free()
{
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
		"assets/Bullets/Melee_07.png",
		1,
		1.0f, {-72 / 2,0},
		0.06f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		8,
		1.0f, {-72 / 2,0},
		0.1f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniBulletDisappear,
	},
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		29,
		1.0f, {-50 / 2,0},
		0.1f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniBulletDisappear,
	},
	{
		"assets/Bullets/MosqueMissile_%02d.png",
		3,
		1.0f, {-40 / 2,0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
};




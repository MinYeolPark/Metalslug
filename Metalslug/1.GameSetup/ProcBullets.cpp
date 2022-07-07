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

	rp = p;
	d = 0, _d = 0;
	degree = 0;

	pow = 0.f;
	up = 0.f;
	down = 0.f;

	speed = 0;
	damage = 0;

	pattern = NULL;

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

void ProcBullets::init(ProcObject* parent, int index, float degree)
{
	this->isActive = true;
	this->parent = parent;
	this->index = index;
	this->p = parent->p;
	this->degree = degree;
	this->alpha = 1.0f;
	BulletPattern bp[BulletIndexMax] = {
	ProcBulletsPattern::patternHandgun,
	ProcBulletsPattern::patternHeavyMachinegun,
	ProcBulletsPattern::patternBomb,

	ProcBulletsPattern::patternMelee,
	ProcBulletsPattern::patternMosque,
	ProcBulletsPattern::patternMosqueTrace,
	ProcBulletsPattern::patternMeleeEnd,
	};
	iSize bs[BulletIndexMax] =
	{
		{10, 5},
		{25, 5},
		{10, 5},
		{30, 30},
		{10, 5},
		{25, 5},
		{25, 5},
	};
#if 1
	colNum = 1;
	for (int i = 0; i < colNum; i++)
		colliders[i]->init(parent, bs[index]);
#endif	
	this->pattern = bp[index];
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);	
	printf("v=%f, %f\n", v.x, v.y);
}

void ProcBullets::init(ProcPlayer* parent, int index, float degree)
{
	ProcBullets::init((ProcObject*)parent, index, degree);	

	this->p = parent->fp;
	this->speed = player->curGun->speed;
	this->damage = player->curGun->dmg;
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
	ProcBullets::init((ProcObject*)parent, index, degree);
	this->p = parent->fp;
	if (index == BulletMosque)
		speed = 30;
	else
		this->speed = 100;
	this->damage = 100;
	if (index == BulletMelee)
	{
		pow = 5.f;
		up -= pow;
		down = 0.0f;		
		//imgs[BulletMeleeEnd]->startAnimation(AnimationMgr::cbAniBulletDisappearWithAlpha, this);
	}
}

void ProcBullets::update(float dt)
{	
	printf("collider[0] p=%f, width =%f\n", colliders[0]->p.x, colliders[0]->s.width);

#if 1
	isActive = containPoint(p,
	iRectMake(-map->off.x - 20, -map->off.y - 20,
		devSize.width + 40, devSize.height + 40));
#endif
	if(pattern!=NULL)
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
	
	for (int i = 0; i < colNum; i++)
		colliders[i]->update(p, degree, dt);
	//collider->setPosition(p);	
	//if (degree == 90 || degree == 270)
	//	collider->setSize(
	//		iSizeMake(collider->getCollider().size.height, collider->getCollider().size.width));
	//else
	//	collider->setSize(
	//		iSizeMake(collider->getCollider().size.width, collider->getCollider().size.height));
}

bool ProcBullets::draw(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, alpha);
	imgCurr = imgs[index];
	imgCurr->degree = degree;			
	imgCurr->paint(dt, rp + off);
#ifdef _DEBUG				
	setDotSize(5);
	drawDot(rp + off);
	drawDot(p + off);
	iRect c = colliders[0]->getCollider();
	c.origin.x += off.x;
	c.origin.y += off.y;
	drawRect(c);	
#endif // DEBUG
	
	setRGBA(1, 1, 1, 1);
	return !isActive;
}

void ProcBullets::free()
{
}

void ProcBullets::getDamage(float damage, Collider* c)
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
		"assets/Bullets/MidBoss_Fire_%02d.png",
		30,
		1.0f, {-72 / 2,0},
		0.1f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/Bullets/MosqueTrace_%02d.png",
		3,
		1.0f, {-40 / 2,0},
		0.1f,
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




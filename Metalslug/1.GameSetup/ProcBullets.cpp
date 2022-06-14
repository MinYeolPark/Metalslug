#include "ProcBullets.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "BulletMgr.h"

static iImage** _imgBullets = NULL;
ImageInfo bulletImageInfo[];
ProcBullets::ProcBullets(int idx) : ProcObject(idx)
{
	imgs = NULL;
	imgCurr = NULL;
	parent = NULL;
	bulletIdx = idx;

	v = iPointZero;

	pattern = NULL;
	speed = 0;
	dmg = 0;

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

void ProcBullets::initObj(Mosque* parent, int idx, iPoint p, float degree)
{
	this->isActive = true;
	this->parent = (ProcObject*)parent;
	this->bulletIdx = idx;
	this->p = p;
	this->degree = degree;
	speed = 50;
	dmg = 100;
	if (bulletIdx == BulletMosqueTrace)
		this->pattern = patMosqueTrace;
	else
		this->pattern = patMosque;

	imgs[this->bulletIdx]->startAnimation(cbAniBullet, this);
}



void ProcBullets::initObj(ProcObject* parent, int idx, iPoint p, float degree)
{
}

void ProcBullets::initObj(ProcEnemy* parent, int idx, iPoint p, float degree)
{
}

void ProcBullets::initObj(ProcPlayer* parent, int idx, iPoint p, float degree)
{
	this->isActive = true;
	this->parent = parent;
	this->bulletIdx = idx;	
	this->p = parent->firePoint;
	this->degree = degree;
	this->pattern = patDefault;
	speed = 300;
	dmg = 100;
	imgs[this->bulletIdx]->startAnimation();
}

void ProcBullets::updateObj(float dt)
{
#if 1
	isActive = containPoint(p,
	iRectMake(-bg->off.x - 20, -bg->off.y - 20,
		devSize.width + 40, devSize.height + 40));
#endif
	pattern(this, dt);
	//Pattern
	fixedUpdate(dt);
}

void ProcBullets::fixedUpdate(float dt)
{
	if (!parent)
		return;
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
			if (eNear->getState() != (EnemyBehave)(DeadEnemyL + eNear->state % 2))
			{
				isActive = false;
				eNear->hp -= dmg;				
				iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
				if (eNear->hp <= 0)
					eNear->setState((EnemyBehave)(DeadEnemyL + eNear->state % 2));

				//Particles
				addProcEffect(bulletIdx, bp);		//bulletIndex=effectIndex
			}
		}
	}
}

bool ProcBullets::drawObj(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, 1);

	imgCurr = imgs[bulletIdx];
	imgCurr->paint(dt, p + off);

	setRGBA(1, 1, 1, 1);

#ifdef _DEBUG		
	drawDot(p + off);
#endif // DEBUG
	
	return !isActive;
}

void ProcBullets::freeObj()
{
}

iRect ProcBullets::collider()
{
	return iRectMake(p.x + bg->off.x - imgCurr->tex->width / 2,
		p.y + bg->off.y - imgCurr->tex->height / 2,
		imgCurr->tex->width, imgCurr->tex->height);
}

void ProcBullets::patDefault(ProcBullets* b, float dt)
{
	//degree?
	b->v = iPointRotate(iPointMake(1, 0), iPointZero, 0);

	b->p += b->v * b->speed * dt;
}

static float r = 0;
void ProcBullets::patBomb(ProcBullets* b, float dt)
{

#if 0
	dt = 1.0 / (b->speed * cos(45));
	b->v.x = b->speed * cos(45);
	b->v.y = b->speed * sin(45);

	int maxY = (*bg->maxY + int(b->p.x));
	while (b->p.y < maxY)
	{
		b->v.x = b->v.x * dt;
		b->v.y = (9.81 + b->v.y) * dt;
		b->p.x += b->v.x * dt;
		b->p.y += b->v.y * dt;
	}
#elif 0
	b->v = iPointRotate(iPointMake(0, -1), iPointZero, degree) * b->speed;
#elif 0
	float td = iPointLength(b->p - iPointMake(player->firePoint.x + 50, player->firePoint.y));

	float vx = sqrt(b->speed) * _cos(45);
	float vy = sqrt(b->speed) * _sin(45);

	//airborn time
	float duration = td / vx;
	float elaspe_time = 0;
	while (elaspe_time < duration)
	{
		elaspe_time += dt;
	}
#elif 0
	b->v = iPointRotate(iPointMake(1, 0), iPointZero, 45) * b->speed;
#elif 0	
	float maxY = *(bg->maxY + (int)b->p.x);
	player->targetPoint = tp;
	
	//if (movePoint(b->p, b->p, player->targetPoint, player->bombSpeed))
	//	b->isActive = false;
	iPoint v = (player->targetPoint, b->p);
	v /= iPointLength(v);
#elif 1
	float maxY = *(bg->maxY + (int)b->p.x);
	iPoint tp = { player->p.x + 150, maxY };

	iPoint sp = player -> p;
	iPoint ep = tp;

	iPoint v = (ep - sp);
	v /= iPointLength(v);
	v *= 3;
	if (b->p.x < ep.x)
	{
		b->p.x += v.x;
		if (b->p.x > ep.x)
			b->p.x = ep.x;
	}
	else if (b->p.x > ep.x)
	{
		b->p.x += v.x;
		if (b->p.x < ep.x)
			b->p.x = ep.x;
	}
	if (b->p.y < ep.y)
	{
		for (int i = 0; i < 180; i ++)
		{			
			if (i > 90)
				b->p.y += _sin(i) * 2;
			else
				b->p.y -= _sin(i) * 2;
		}	
	}
#endif
}
void ProcBullets::patMosque(ProcBullets* b, float dt)
{
	
}

void ProcBullets::patMosqueTrace(ProcBullets* b, float dt)
{
	iPoint v = player->p - b->p;
	v /= iPointLength(v);
	v *= (b->speed * dt);
	b->v = iPointRotate(v, player->p, 180);	
	b->p += v;
	if (player)
	{
		if (containPoint(b->p, player->collider()))
		{
			b->isActive = false;
			//player->hp -= b->dmg;

			//dead
			//addProcEffect(bulletIdx, bp);
		}	
	}
}

void ProcBullets::cbAniBullet(void* parm)
{
	ProcBullets* b = (ProcBullets*)parm;
	b->isActive = false;

	addBullet(b->m, BulletMosqueTrace, b->p);
}

ImageInfo bulletImageInfo[BulletIndexMax] =
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
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		29,
		1.0f, {-50 / 2,0},
		0.1f,
		1,
		{255, 0, 0, 255},
		ProcBullets::cbAniBullet,
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




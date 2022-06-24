#include "ProcBullets.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "BulletMgr.h"

#include "Kessie.h"
static iImage** _imgBullets = NULL;
ImageInfo bulletImageInfo[];
ProcBullets::ProcBullets(int idx)
{
	imgs = NULL;
	imgCurr = NULL;
	parent = NULL;
	bulletIdx = idx;

	v = iPointZero;
	collider = new Collider();
	collider->isActive = true;
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
	this->pattern = patternDefault;
	this->speed = player->curGun->speed;
	dmg = player->curGun->dmg;
	v = iPointRotate(iPointMake(1, 0), iPointZero, degree);
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
	if (parent->layer == LayerPlayer)
	{
		ProcObject* oNear = NULL;
		float dNear = 0xffffff;
		for (int i = 0; i < objects->count; i++)
		{
			ProcObject* o = (ProcObject*)objects->objectAtIndex(i);
			if (o->layer != LayerPlayer)
			{
				if (containPoint(p, o->collider->getCollider()))
				{
					float d = iPointLength(p - o->p);
					if (dNear > d)
					{
						if (o->collider->isActive)
						{
							dNear = d;
							oNear = o;
						}
					}
				}
			}
#if 0
			Kessie* k = (Kessie*)objects->objectAtIndex(i);
			if (containPoint(p, k->leftCollider->getCollider()) ||
				containPoint(p, k->rightCollider->getCollider()))
			{
				float d = iPointLength(p - k->p);
				if (dNear > d)
				{
					if (k->leftCollider->isActive)
					{
						dNear = d;
						oNear = o;
					}
					if (k->rightCollider->isActive)
					{
						dNear = d;
						oNear = o;
					}
				}
			}
#endif
		}
		if (oNear)
		{
			if (oNear->isActive)
			{
				isActive = false;
				oNear->getDamage(dmg);
				iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
				addProcEffect(bulletIdx, bp);		//bulletIndex=effectIndex
			}
		}
	}
	
#if 0
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
#endif
}

bool ProcBullets::drawObj(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, 1);

	imgCurr = imgs[bulletIdx];
	imgCurr->paint(dt, p + off);


#ifdef _DEBUG			
	
#endif // DEBUG
	
	setRGBA(1, 1, 1, 1);
	return !isActive;
}

void ProcBullets::freeObj()
{
}
//
//iRect ProcBullets::collider()
//{
//	return iRectMake(p.x + bg->off.x - imgCurr->tex->width / 2,
//		p.y + bg->off.y - imgCurr->tex->height / 2,
//		imgCurr->tex->width, imgCurr->tex->height);
//}

void ProcBullets::patternDefault(ProcBullets* b, float dt)
{
	b->p += b->v * b->speed * dt;
}

static float r = 0;
void ProcBullets::patBomb(ProcBullets* b, float dt)
{

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
		if (containPoint(b->p, player->collider->getCollider()))
		{
			b->isActive = false;
			player->hp -= b->dmg;

			//dead
			//addProcEffect(bulletIdx, bp);
		}	
	}
}

void ProcBullets::cbAniBullet(void* parm)
{
	ProcBullets* b = (ProcBullets*)parm;
	b->isActive = false;

	//addBullet(b->m, BulletMosqueTrace, b->p);
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
	///////////////////////////////////////////////////////
	{
		"assets/Bullets/MidBoss_Fire_%02d.png",
		8,
		1.0f, {-72 / 2,0},
		0.1f,
		1,
		{255, 0, 0, 255},
		ProcBullets::cbAniBullet,
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




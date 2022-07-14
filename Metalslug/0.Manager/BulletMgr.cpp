#include "BulletMgr.h"

#include "EffectMgr.h"

#include "ProcField.h"
#include "BulletsPlayer.h"
#include "BulletsEnemy.h"
#include "BulletsMosque.h"
ProcBullets*** _bullets;
ProcBullets** bullets;
int bulletNum;

void loadProcBullets()
{
	_bullets = new ProcBullets * *[BulletIndexMax];
	for (int i = 0; i < BulletIndexMax; i++)
	{
		_bullets[i] = new ProcBullets * [bulletMax];
		if (i == BulletHandGun || i == BulletHeavyMachinegun)
		{
			for (int j = 0; j < bulletMax; j++)
				_bullets[i][j] = new BulletsPlayer(i);
		}
		else if (i == BulletMelee)
		{
			for (int j = 0; j < bulletMax; j++)
				_bullets[i][j] = new BulletsEnemy(i);
		}
		else if (i == BulletMosque || i == BulletMosqueTrace)
		{
			for (int j = 0; j < bulletMax; j++)
				_bullets[i][j] = new BulletsMosque(i);
		}
	}
	bullets = new ProcBullets * [BulletIndexMax * bulletMax];
	bulletNum = 0;
}

void freeProcBullets()
{
	for (int i = 0; i < bulletMax; i++)
		delete _bullets[i];
	delete _bullets;

	delete bullets;
}

void drawProcBullets(float dt, iPoint off)
{
	for (int i = 0; i < bulletNum; i++)
	{
		ProcBullets* b = bullets[i];
		b->update(dt);		
		if (b->draw(dt, off))
		{
			bulletNum--;
			bullets[i] = bullets[bulletNum];
			i--;
		}		
	}
}

void addBullet(ProcObject* parent, int index, float degree, int fpNum)
{
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[index][i];
		if (b->isActive == false)
		{
			b->init(parent, index, degree, fpNum);
			bullets[bulletNum] = b;
			bulletNum++;
			return;
		}
	}
}

void ProcBulletsPattern::patternHandgun(ProcBullets* b, float dt)
{	
	b->p += b->v * b->speed * dt;
}

void ProcBulletsPattern::patternHeavyMachinegun(ProcBullets* b, float dt)
{
	b->p += b->v * b->speed * dt;
}

void ProcBulletsPattern::patternBomb(ProcBullets* b, float dt)
{	
	b->up -= b->pow;

	if (b->up)
	{
		b->p = iPointMake(b->p.x, b->p.y -= b->pow);
		b->up += 9.81 * dt;
	}

	int maxY = *(map->maxY + (int)b->p.x);
	if (b->p.y < maxY)
	{
		b->down += 9.81 * dt;		
	}
}

void ProcBulletsPattern::patternMelee(ProcBullets* b, float dt)
{
	if (b->up)
	{
		b->p = iPointMake(b->p.x, b->p.y -= b->pow);
		b->up += 9.81 * dt;
		if (b->up > 0.0f)
			b->up = 0.0f;
	}

	int maxY = *(map->maxY + (int)b->p.x);
	if (b->p.y < maxY)
	{
		b->down += 9.81 * dt;
		if (b->parent->isRight)
			b->p = iPointMake(b->p.x + 2, b->p.y += b->down);
		else// if(!b->parent->isRight)
			b->p = iPointMake(b->p.x - 2, b->p.y += b->down);
	}
	else
	{
		b->index = BulletMeleeEnd;
		//for (int i = 0; i < b->colNum; i++)
		//	b->colliders[i]->isActive = false;
		b->up = 0.0f;
		b->pattern = patternMeleeEnd;		
		return;
	}
}

void ProcBulletsPattern::patternMeleeEnd(ProcBullets* b, float dt)
{	
	b->alpha -= dt;
	if (b->alpha < 0.0f)
	{
		b->alpha = 0.0f;
		b->isActive = false;
	}
}

void ProcBulletsPattern::patternMosque(ProcBullets* b, float dt)
{	
	if (b->imgs[b->index]->frame == 29)
	{
		b->index = BulletMosqueTrace;
		b->pattern = patternMosqueTrace;
	}
}

#define wave 15				//ÁøÆø
static float _d = 100.f;
void ProcBulletsPattern::patternMosqueTrace(ProcBullets* b, float dt)
{
	iPoint md = b->v * b->speed * dt;
	b->p += md;

	/*b->d += iPointLength(md);
	if (b->d > _d)
		b->d -= _d;
	float r = b->d / _d;		*/
	//float angle = iPointAngle(iPointMake(1, 0), iPointZero, b->v);			
	//iPoint w = iPointRotate(iPointMake((wave * _sin(360 * r)), 0), iPointZero, angle + 90);		
	//b->degree = 45 * _sin(360 * r);	
	//b->rp = b->p + w;	
}

#include "BulletMgr.h"

#include "EffectMgr.h"
ProcBullets*** _bullets;
ProcBullets** bullets;
int bulletNum;
void loadProcBullets()
{
	_bullets = new ProcBullets * *[BulletIndexMax];
	for (int i = 0; i < BulletIndexMax; i++)
	{
		_bullets[i] = new ProcBullets * [bulletMax];
		for (int j = 0; j < bulletMax; j++)
			_bullets[i][j] = new ProcBullets(i);
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

void addBullet(ProcPlayer* parent, int index, float degree)
{
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[index][i];
		if (b->isActive == false)
		{
			b->init(parent, index, degree);
			bullets[bulletNum] = b;
			bulletNum++;
			return;
		}
	}
}

void addBullet(ProcEnemy* enemy, int index, float degree)
{
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[index][i];
		if (b->isActive == false)
		{
			b->init(enemy, index, degree);
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
		b->collider->isActive = false;
		b->up = 0.0f;
		b->pattern = patternMeleeEnd;		
		return;
	}
}

void ProcBulletsPattern::patternMeleeEnd(ProcBullets* b, float dt)
{	
	b->alpha -= dt;
	printf("alpha=%f\n", b->alpha);
	if (b->alpha < 0.0f)
	{
		b->alpha = 0.0f;
		b->isActive = false;
	}
}

void ProcBulletsPattern::patternMosque(ProcBullets* b, float dt)
{
	if (b->index == BulletMosque)
	{
		if (b->imgs[b->index]->frame == 29)
		{
			b->index = BulletMosqueTrace;
			b->pattern = patternMosqueTrace;
		}
	}
}

static float tDegree = 60.f;
void ProcBulletsPattern::patternMosqueTrace(ProcBullets* b, float dt)
{
#if 1	
	if (tDegree > 0)
	{
		b->degree += b->speed * dt;
		if (b->degree > tDegree)
			tDegree *= -1;
	}
	else if (tDegree < 0)
	{
		b->degree -= b->speed * dt;
		if (b->degree < tDegree)
			tDegree *= -1;
	}
	iPoint v = player->p - b->p;
	v /= iPointLength(v) * b->speed * dt;
	b->p.x += _cos(b->degree);
	b->p.y += _sin(b->degree);
	//b->p.x += v.x;
	setRGBA(1, 1, 1, 1);
	drawDot(b->p + map->off);
	setRGBA(1, 1, 1, 1);
#else
	iPoint v = player->p - b->p;
	v /= iPointLength(v);
	v *= (b->speed * dt);	 
	if (b->degree < 90)
	{
		b->degree += (90 / 0.5f * dt);
		//if (degree > 90)
		//	degree = 90;
	}
	else if (b->degree > 90)
	{
		b->degree -= (90 / 0.5f * dt);
		//if (degree < 90)
		//	degree = 90;
	}
	b->v = iPointRotate(v, b->parent->tp, b->degree);
	b->p += v;
	if (player)
	{
		for (int i = 0; i < player->colNum; i++)
		{
			if (containPoint(b->p, player->colliders[i]->getCollider()))
			{
				addProcEffect(b->index, b->p);
				player->getDamage(b->damage, player->colliders[i]);
				b->isActive = false;
				//dead
			}
		}
	}
#endif
}

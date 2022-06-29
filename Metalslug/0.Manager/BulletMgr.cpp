#include "BulletMgr.h"

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
}

void ProcBulletsPattern::patternBomb(ProcBullets* b, float dt)
{
}

void ProcBulletsPattern::patternMelee(ProcBullets* b, float dt)
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
		b->p = iPointMake(b->p.x - 2.5, b->p.y += b->down);
	}
	else if (b->p.y > maxY)
	{
		b->up = 0.0f;
		b->down = 0.0f;
	}

	if (b->p.y > maxY)
	{
		b->up = 0.0f;
		b->isActive = false;
		//b->index = BulletMeleeEnd;
	}
}

void ProcBulletsPattern::patternMosque(ProcBullets* b, float dt)
{
	iPoint v = player->p - b->p;
	v /= iPointLength(v);
	v *= (b->speed * dt);
	b->v = iPointRotate(v, player->p, 180);
	b->p += v;
	if (player)
	{
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(b->p, player->colliders[i]->getCollider()))
			{
				b->isActive = false;
				player->hp -= b->damage;

				//dead
				//addProcEffect(bulletIdx, bp);
			}
		}
	}
}

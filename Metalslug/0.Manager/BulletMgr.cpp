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
		b->updateObj(dt);
		if (b->drawObj(dt, off))
		{
			bulletNum--;
			bullets[i] = bullets[bulletNum];
			i--;
		}
	}
}

void addBullet(Mosque* m, int idx, iPoint p)
{
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[idx][i];
		if (b->isActive == false)
		{
			b->initObj(m, idx, p, 0);

			bullets[bulletNum] = b;
			bulletNum++;
			return;
		}
	}
}

void addBullet(ProcPlayer* parent, int idx, float degree)
{
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[idx][i];
		if (b->isActive == false)
		{
			b->initObj(parent, idx, parent->p, degree);

			bullets[bulletNum] = b;
			bulletNum++;
			return;
		}
	}
}
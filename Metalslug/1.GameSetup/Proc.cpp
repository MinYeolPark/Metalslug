﻿#include "Proc.h"

#include "Loading.h"

#include "ProcField.h"
#include "ProcBullets.h"
void loadProc()
{
	loadProcField();
	//loadProcPlayer();
	loadProcBullets();
}
void freeProc()
{
	freeProcField();
	//freeProcPlayer();
	freeProcBullets();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	//drawProcPlayer(dt, off);
	drawProcBullets(dt, off);
	setRGBA(1, 1, 1, 1);
}

bool keyProc(iKeyState stat, iPoint p)
{
	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

///////////////////////////////////////////////////
//Field
///////////////////////////////////////////////////
void loadProcField()
{
	bg = new Bg();
}

void freeProcField()
{
	delete bg;
}

iPoint drawProcField(float dt)
{
	iPoint off = bg->off;
	bg->paint(dt);

	return off;

	addBullet(NULL, 0, 0);
}


///////////////////////////////////////////////////
//Bullets
///////////////////////////////////////////////////

ProcBullets*** _bullets;
ProcBullets** bullets;
int bulletNum;
static iImage*** _imgProcBullet;
void loadProcBullets()
{
	_imgProcBullet = NULL;

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
	{
		delete _imgProcBullet[i];
		delete _bullets[i];
	}
	delete _imgProcBullet;
	delete _bullets;

	delete bullets;
	_imgProcBullet = NULL;
}

void drawProcBullets(float dt, iPoint off)
{
	for (int i = 0; i < bulletNum; i++)
	{
		ProcBullets* b = bullets[i];
		b->updateObj(dt);
		b->drawObj(dt, b->p);
	}
}

void addBullet(ProcObject* parent, int idx, int dir)
{
#if 0
	for (int i = 0; i < bulletMax; i++)
	{
		ProcBullets* b = _bullets[idx][i];
		if (b->isActive == false)
		{
			b->isActive = true;
			int d[4] = { 0, 180, 90, 270 };
#if 0
			b->p = parent->p + iPointRotate(parent->firePoint, iPointZero, 360 - d[dir]);
#else
			if (parent->layer == OBJECT_LAYER::Player)
			{
				ProcPlayer* own = (ProcPlayer*)parent;
				float y = own->firePoint.y;
				own->firePoint.y = 0;
				iPoint mp = iPointRotate(own->firePoint, iPointZero, 360 - d[dir]);
				mp.y += y;
				own->firePoint.y = y;
				b->p = parent->p + mp;
			}
			if (parent->layer == OBJECT_LAYER::Enemy)
				parent = (ProcEnemy*)parent;
#endif
			b->parent = parent;
			b->v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - d[dir]);
			bullets[bulletNum] = b;
			bulletNum++;
			return;
		}
	}
#endif
}


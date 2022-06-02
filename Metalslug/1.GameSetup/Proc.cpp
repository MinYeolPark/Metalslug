#include "Proc.h"

#include "GameMgr.h"
#include "InputMgr.h"
#include "UIMgr.h"

#include "Loading.h"

#include "ProcField.h"
#include "ProcBullets.h"

void loadProc()
{
	loadProcField();
	loadProcPlayer();
	loadProcBullets();
	loadProcEnemy();

	loadUI();
}
void freeProc()
{
	freeProcField();
	freeProcPlayer();
	freeProcBullets();
	freeProcEnemy();

	freeUI();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	drawProcPlayer(dt, off);
	drawProcBullets(dt, off);
	drawProcEnemy(dt, off);
	drawUI(dt, off);

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
}

///////////////////////////////////////////////////
//Player
///////////////////////////////////////////////////

void loadProcPlayer()
{
	player = new ProcPlayer();

	player->initObj();
}

void freeProcPlayer()
{
	delete player;
}

void drawProcPlayer(float dt, iPoint off)
{
	if (player->isActive)
	{
		player->updateObj(dt);
		player->drawObj(dt, off);
	}
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
		b->drawObj(dt, off);
	}
}

void addBullet(ProcObject* parent, int idx, int dir)
{
	for (int i = 0; i < bulletMax; i++)
	{
		printf("bulletNum[%d]\n", bulletNum);
		ProcBullets* b = _bullets[idx][i];
		if (b->isActive == false)
		{
			b->isActive = true;
			b->initObj();

			b->parent = parent;	
			b->bulletIdx = idx;
			b->dir = dir;
			bullets[bulletNum] = b;
			bulletNum++;
			return;

		}
	}
}





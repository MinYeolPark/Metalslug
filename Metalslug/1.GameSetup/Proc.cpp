#include "Proc.h"

#include "GameMgr.h"
#include "InputMgr.h"
#include "UIMgr.h"

#include "EffectMgr.h"
#include "BulletMgr.h"

#include "Loading.h"

#include "ProcField.h"


void loadProc()
{
	loadProcField();
	loadProcEnemy();
	loadProcBullets();
	loadProcEffect();
	loadProcPlayer();

	loadUI();
}
void freeProc()
{
	freeProcField();
	freeProcEnemy();
	freeProcBullets();
	freeProcEffect();
	freeProcPlayer();

	freeUI();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	drawProcEnemy(dt, off);
	drawProcBullets(dt, off);
	drawProcEffect(dt, off);
	drawProcPlayer(dt, off);
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
	player->updateObj(dt);
	if (player->drawObj(dt, off))
	{
		
	}
}







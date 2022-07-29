#include "Proc.h"

#include "GameMgr.h"
#include "InputMgr.h"
#include "UIMgr.h"

#include "EffectMgr.h"
#include "BulletMgr.h"

#include "Select.h"
#include "Loading.h"

#include "ProcField.h"
#include "ProcNpc.h"
#include "ProcItem.h"
#include "ProcStructure.h"
#include "ProcNpc.h"

void loadProc()
{
	loadProcField();
	loadStructure();
	loadProcPlayer();
	loadProcEnemy();
	loadProcBullets();
	loadProcEffect();


	loadProcItem();
	loadProcNpcs();
	//
}
void freeProc()
{
	freeProcField();
	freeStructure();
	freeProcPlayer();
	freeProcEnemy();
	freeProcBullets();
	freeProcEffect();


	freeProcItem();
	freeProcNpcs();
	//
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	drawStructure(dt, off);
	drawProcEnemy(dt, off);
	drawProcItem(dt, off);
	drawProcNpcs(dt, off);
	drawProcBullets(dt, off);
	drawProcEffect(dt, off);
	drawProcPlayer(dt, off);

	drawUI(dt, off);
	setRGBA(1, 1, 1, 1);
}

bool keyProc(iKeyState stat, iPoint p)
{
	if (keyProcUI(stat, p))
		return true;

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
static int stage = 0;		//stage1
void loadProcField()
{
	map = new ProcMap(stage);
	
	map->init(stage);
}

void freeProcField()
{
	delete map;
}

iPoint drawProcField(float dt)
{
	iPoint off = map->off;
	map->update(dt);
	map->paint(dt);

	return off;
}

///////////////////////////////////////////////////
//Player
///////////////////////////////////////////////////

void loadProcPlayer()
{
	//charIdx=selectedBtn;
	//#issue
	player = new ProcPlayer(ERI);
#if 1
	player->init({ 100, 200 });
#elif 0
	player->init({ 1250,200 });
#elif 0
	player->init({ 2100,200 });
#else
	player->init({ 3600, 200 });
#endif
}

void freeProcPlayer()
{
	delete player;
}

float spawnDt;
void drawProcPlayer(float dt, iPoint off)
{
	if (player->topState != (PlayerBehave)(PlayerDead + player->topState % 2)
		&& player->topState != (PlayerBehave(PlayerSpawn + player->topState % 2)))
		player->update(dt);
	if (player->draw(dt, off))
	{
		spawnDt += dt;
		if (spawnDt >= _respawnDt)
		{
			spawnDt -= _respawnDt;
			if (player->life > 0)
				player->init(player->p);
			else//player->life==0
			{			
				if (countDownPopup->bShow)
					return;
				showPopCountdown(true);
			}
		}
	}
}







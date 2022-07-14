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

void loadProc()
{
	loadProcColliders();
	loadProcField();
	loadProcPlayer();
	loadProcEnemy();
	loadProcBullets();

	//loadStructure();

	//loadProcItem();
	//loadProcNpcs();
	//loadProcEffect();
	//
	loadUI();
}
void freeProc()
{
	freeProcColliders();

	freeProcField();
	freeProcPlayer();
	freeProcEnemy();
	freeProcBullets();

	//freeStructure();

	//freeProcItem();
	//freeProcNpcs();
	//freeProcEffect();
	//
	freeUI();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	drawProcEnemy(dt, off);
	drawProcPlayer(dt, off);
	drawProcBullets(dt, off);
	//drawStructure(dt, off);

	//drawProcNpcs(dt, off);
	//drawProcItem(dt, off);
	//drawProcEffect(dt, off);

	drawProcColliders(dt, off);

	drawUI(dt, off);
#if 0
	//#issue
	for (int i = 0; i < objects->count; i++)
	{
		ProcObject* o = (ProcObject*)objects->objectAtIndex(i);
		if (o->isActive == false)
			objects->removeObject(i);
		return;
	}
#endif
	setRGBA(1, 1, 1, 1);
}

bool keyProc(iKeyState stat, iPoint p)
{
	if (keyProcUI(stat, p))
		return true;

	switch (stat) {

	case iKeyStateBegan:
		printf("Clicked point = %f, %f\n", p.x, p.y );
		printf("off = %f, %f\n", map->off.x, map->off.y);
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

#if 0
	player->init({ 3500, 200 });
#elif 1
	player->init({ 100, 200 });
#endif
}

void freeProcPlayer()
{
	delete player;
}

float spawnDt = 0.f;
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
				printf("gameOver\n");
			}
		}
	}
}







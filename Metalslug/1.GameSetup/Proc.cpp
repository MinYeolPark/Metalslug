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

void loadProc()
{
	loadProcField();

	loadProcPlayer();
	loadProcItem();
	loadProcNpcs();
	loadProcEnemy();
	loadProcBullets();
	loadProcEffect();

	loadUI();
}
void freeProc()
{
	freeProcField();

	freeProcPlayer();
	freeProcItem();
	freeProcNpcs();
	freeProcEnemy();
	freeProcBullets();
	freeProcEffect();

	freeUI();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);

	drawProcNpcs(dt, off);
	drawProcItem(dt, off);
	drawProcEnemy(dt, off);
	drawProcBullets(dt, off);
	drawProcPlayer(dt, off);
	drawProcEffect(dt, off);
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
	switch (stat) {

	case iKeyStateBegan:
		printf("Clicked point = %f, %f\n", p.x + map->off.x, p.y + map->off.y);
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

	player->init({100, 200 });
}

void freeProcPlayer()
{
	delete player;
}

float spawnDt = 0.f, _spawnDt = 2.f;
void drawProcPlayer(float dt, iPoint off)
{
	if (player->topState != (PlayerBehave)(PlayerDead + player->topState % 2)
		&& player->topState != (PlayerBehave(PlayerSpawn + player->topState % 2)))
		player->update(dt);
	if (player->draw(dt, off))
	{
		//player->life--;
		spawnDt += dt;
		if (spawnDt >= _spawnDt)
		{
			spawnDt -= _spawnDt;
			if (player->life > 0)
				player->init(player->p);
			else//player->life==0
				;//gameover
		}
	}
}







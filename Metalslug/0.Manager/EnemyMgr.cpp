#include "EnemyMgr.h"
#include "BulletMgr.h"
#include "EffectMgr.h"

#include "Mosque.h"
#include "Kessie.h"
#include "ArabMelee.h"
#include "ArabBurserker.h"
#include "ArabCamel.h"
#include "Truck.h"
#include "Abul.h"
#include "Kessie.h"
#include "ProcPlayer.h"
#include "ProcField.h"

ProcEnemy*** _enemies;
ProcEnemy** enemies;
int enemyNum;

ArabCamel* arabCamel;
Truck* truck;
Kessie* kessie;
Mosque* mosque;
Abul* abul;
int maxNum[EnemyIndexMax] =
{
	50,		//melee
	20,		//kessie melee
	1,		//mosque
	50,		//burser
	1,		//camel
	1,		//truck
	1,		//kessie
	1,		//abul
};

static bool truckTrigger = false;
void loadProcEnemy()
{
	_enemies = new ProcEnemy * * [EnemyIndexMax];
	for (int i = 0; i < EnemyIndexMax; i++)
	{		
		_enemies[i] = new ProcEnemy * [maxNum[i]];
		if (i == IdxMelee || i==IdxMeleeKessie)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
		else if (i == IdxMosque)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Mosque(i);
		}
		else if (i == IdxBurserker)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new ArabBurserker(i);
		}
		else if (i == IdxArCamel)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new ArabCamel(i);
		}
		else if (i == IdxTruck)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Truck(i);
		}
		else if (i == IdxKessie)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Kessie(i);
		}

		else if (i == IdxAbul)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Abul(i);
		}
		enemies = new ProcEnemy * [EnemyIndexMax *  maxNum[i]];		
	}
	enemyNum = 0;

	addProcEnemy(IdxMosque, iPointMake(2030, 175));	
	addProcEnemy(IdxTruck, iPointMake(1700, 100));
	addProcEnemy(IdxMelee, iPointMake(500, 50));
	addProcEnemy(IdxBurserker, iPointMake(1300, 100));
	addProcEnemy(IdxBurserker, iPointMake(1350, 100));
	addProcEnemy(IdxBurserker, iPointMake(1400, 100));


	//addProcEnemy(IdxArMelee, iPointMake(400, 100), iPointMake(0, 0));
	//addProcEnemy(IdxArCamel, iPointMake(300, 100), iPointMake(0, 0));
	addProcEnemy(IdxKessie, iPointMake(3680, -50));
}
void freeProcEnemy()
{
	for (int i = 0; i < EnemyIndexMax; i++)
	{
		for (int j = 0; j < maxNum[i]; j++)
		{
			_enemies[i][j]->free();
			delete _enemies[i][j];
		}
		delete _enemies[i];
	}
	delete _enemies;
	delete enemies;
}

void drawProcEnemy(float dt, iPoint off)
{
	for (int i = 0; i < enemyNum; i++)
	{
		ProcEnemy* e = enemies[i];
		if (e->isActive)
		{
			e->update(dt);
			if (e->draw(dt, off))
			{
				enemyNum--;
				enemies[i] = enemies[enemyNum];
				i--;
			}
		}
	}


	if (!truckTrigger)
	{		
		if (player->p.x > 1550)
		{
			truckTrigger = true;
			addProcEnemy(IdxAbul, iPointMake(1700, 150));
		}
	}
}

ProcEnemy* addProcEnemy(int index, iPoint p)
{
	for (int i = 0; i < maxNum[i]; i++)
	{
		ProcEnemy* e = _enemies[index][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->init(p);			//default direction = Left
			enemies[enemyNum] = e;
			enemyNum++;
			return e;
		}
	}
}

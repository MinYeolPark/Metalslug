#include "EnemyMgr.h"

#include "ArabMelee.h"
#include "ArabBurserker.h"
ProcEnemy*** _enemies;
ProcEnemy** enemies;

int enemyCount;

void loadProcEnemy()
{
	_enemies = new ProcEnemy * *[(int)EnemyIndex::EnemyIndexMax];
	for (int i = 0; i < (int)EnemyIndex::EnemyIndexMax; i++)
	{
		_enemies[i] = new ProcEnemy * [enemyMax];
		if (i == 0)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
	//	}
	//	else if (i == 1)
	//	{
	//		//for (int j = 0; j < enemyMax; j++)
	//		//	_enemies[i][j] = new ArabBurserker(i);
	//	}
	}
	enemies = new ProcEnemy * [(int)EnemyIndex::EnemyIndexMax * enemyMax];
	enemyCount = 0;

	addProcEnemy((int)EnemyIndex::ArMelee, iPointMake(200, 50));
}
void freeProcEnemy()
{
	for (int i = 0; i < (int)EnemyIndex::EnemyIndexMax; i++)
	{
		for (int j = 0; j < enemyMax; j++)
		{
			_enemies[i][j]->freeObj();
			delete _enemies[i][j];
		}
		delete _enemies[i];
	}
	delete _enemies;
	delete enemies;
}
void drawProcEnemy(float dt, iPoint off)
{
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];

		if (e->isActive)
		{
			//if (e->getState() != (EnemyBehave)(eDeadL + (e->state % 2)))
			e->updateObj(dt);
			e->drawObj(dt, off);
			e->updateAi(e, dt);
		}

		if (e->isActive == false)
		{
			enemyCount--;
			enemies[i] = enemies[enemyCount];
			i--;
		}

	}
}
void addProcEnemy(int idx, iPoint p)
{
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[idx][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->p = p;
			e->idx = (EnemyIndex)idx;
			e->initObj();
			enemies[enemyCount] = e;
			enemyCount++;
			return;
		}
	}
}

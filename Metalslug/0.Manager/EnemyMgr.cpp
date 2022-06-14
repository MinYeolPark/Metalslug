#include "EnemyMgr.h"

#include "Mosque.h"

#include "ArabMelee.h"
#include "ArabBurserker.h"

/////////////////////////////////
Mosque* mosque;
/////////////////////////////////
ProcEnemy*** _enemies;
ProcEnemy** enemies;
int enemyCount;
void loadProcEnemy()
{
	_enemies = new ProcEnemy * * [EnemyIndexMax];
	for (int i = 0; i < EnemyIndexMax; i++)
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
	enemies = new ProcEnemy * [EnemyIndexMax * enemyMax];
	enemyCount = 0;

	addProcEnemy(ArMelee, iPointMake(200, 50));


#if 1
	mosque = new Mosque();
	mosque->initObj();
#endif

}
void freeProcEnemy()
{
	for (int i = 0; i < EnemyIndexMax; i++)
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

#if 1
	delete mosque;
#endif
}
void drawProcEnemy(float dt, iPoint off)
{
#if 1
	mosque->updateObj(dt);
	mosque->drawObj(dt, off);
#endif
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

#include "EnemyMgr.h"
#include "BulletMgr.h"

#include "Mosque.h"
#include "Kessie.h"
#include "ArabMelee.h"
#include "ArabBurserker.h"
#include "ArabCamel.h"

#include "Kessie.h"
#include "ProcPlayer.h"
#include "ProcField.h"

/////////////////////////////////
Mosque* mosque;
Kessie* kessie;
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
		if (i == IdxArMelee)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
		else if (i == IdxArBurserker)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabBurserker(i);
		}
		else if (i == IdxArCamel)
		{
			for (int j = 0; j < 1; j++)
				_enemies[i][j] = new ArabCamel(i);
		}
		else if (i == IdxKessie)
		{
			for (int j = 0; j < 1; j++)
				_enemies[i][j] = new Kessie(i);
		}

	}
	enemies = new ProcEnemy * [EnemyIndexMax * enemyMax];
	enemyCount = 0;

	addProcEnemy(IdxArMelee, iPointMake(250, 50), iPointMake(0,0));	
	addProcEnemy(IdxArBurserker, iPointMake(200, 50), iPointMake(0, 0));
	//addProcEnemy(IdxArCamel, iPointMake(300, 50), iPointMake(0, 0));
	addProcEnemy(IdxKessie, iPointMake(200, 50), iPointMake(0, 0));
#if 0
	mosque = new Mosque();
	mosque->initObj();

	kessie = new Kessie();
	kessie->init();
#endif

}
void freeProcEnemy()
{
	for (int i = 0; i < EnemyIndexMax; i++)
	{
		for (int j = 0; j < enemyMax; j++)
		{
			_enemies[i][j]->free();
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
#if 0
	mosque->updateObj(dt);
	mosque->drawObj(dt, off);
#endif
#if 0
	kessie->updateObj(dt);
	kessie->drawObj(dt, off);
#endif
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];
		if (e->isActive)
		{
			if (!e->isDead)
				e->update(dt);
			e->draw(dt, off);
		}

		if (e->isActive == false)
		{			
			enemyCount--;
			enemies[i] = enemies[enemyCount];
			i--;
		}

	}
}

void addProcEnemy(int index, iPoint p, iPoint v)
{
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[index][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->init(index, p, v);			//default direction = Left
			enemies[enemyCount] = e;			
			enemyCount++;
			return;
		}
	}
}

void ProcEnemyAI::ArabMeleeAI0(ProcEnemy* e, float dt)		//Check Player
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp = player->p;	
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v = e->tp - e->p;
		e->v /= iPointLength(e->v);

		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
		}
		else
		{
		}

		if (len < e->attkRange)
		{
		}
	}
}

void ProcEnemyAI::ArabMeleeAI1(ProcEnemy* e, float dt)
{
	//e->p += e->v * e->moveSpeed * dt;
}

void ProcEnemyAI::ArabBurserkAI0(ProcEnemy* e, float dt)		//Spawn From Kessie
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp.x = bg->off.x;
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v = e->tp - e->p;
		e->v /= iPointLength(e->v);

		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
		}
		else
		{
		}
	}
}

void ProcEnemyAI::ArabCamelAI0(ArabCamel* e, float dt)
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp.x = bg->off.x;
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v = e->tp - e->p;
		e->v /= iPointLength(e->v);

		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
			e->arabState = ArabCamelIdleR;
			e->camelState = CamelRunR;
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
			e->arabState = ArabCamelIdleL;
			e->camelState = CamelRunL;
		}	
	}
}

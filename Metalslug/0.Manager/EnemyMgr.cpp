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
int enemyCount;
void loadProcEnemy()
{
	_enemies = new ProcEnemy * * [EnemyIndexMax];
	for (int i = 0; i < EnemyIndexMax; i++)
	{
		_enemies[i] = new ProcEnemy * [enemyMax];
		if (i == IdxArMelee || i==IdxArMeleeKessie)
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
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabCamel(i);
		}
		else if (i == IdxTruck)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new Truck(i);
		}
		else if (i == IdxKessie)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new Kessie(i);
		}
		else if (i == IdxMosque)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new Mosque(i);
		}
		else if (i == IdxAbul)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new Abul(i);
		}
	}
	enemies = new ProcEnemy * [EnemyIndexMax * enemyMax];
	enemyCount = 0;

	//spawn pattern
	
	//addProcEnemy(IdxArMelee, iPointMake(500, 100), iPointMake(0,0));	
	//addProcEnemy(IdxArMelee, iPointMake(400, 100), iPointMake(0, 0));
	//addProcEnemy(IdxMosque, iPointMake(300, 220), iPointZero);
	//addProcEnemy(IdxArBurserker, iPointMake(200, 100), iPointMake(0, 0));
	//addProcEnemy(IdxArCamel, iPointMake(300, 100), iPointMake(0, 0));
	//addProcEnemy(IdxKessie, iPointMake(200, 70), iPointMake(0, 0));

	//addProcEnemy(IdxAbul, iPointMake(300, 100), iPointMake(0, 0));
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
}

static bool isTruckSpawned = false;
void drawProcEnemy(float dt, iPoint off)
{
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];
		if (e->isActive)
		{
			if (e->layer != LayerKessie)
			{
				if (!e->isDead)
					e->update(dt);
			}
			else if (e->layer == LayerKessie)
				e->update(dt);
			else if (e->layer == LayerMosque)
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

#if 0 //spawn pattern
	if (player->p.x > 1350)
	{
		if (!isTruckSpawned)
			addProcEnemy(IdxTruck, iPointMake(1800, 200), iPointMake(-1, 0));
		isTruckSpawned = true;		
	}
#endif
}

void addProcEnemy(int index, iPoint p, iPoint v)
{
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[index][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			printf("index= %d\n", index);
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
		e->setState(FireMeleeL + e->state % 2);
	else if (len < e->attkRange)
		;
}

void ProcEnemyAI::ArabMeleeAI1(ProcEnemy* e, float dt)
{
#if 0
	printf("%f, %f\n", e->tp.x, e->tp.y);

	int len = iPointLength(player->p - e->p);
	if (len < e->sight)
		e->tp.x = player->p.x + map->off.x;
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v.x = player->p.x - e->p.x;
		e->v /= iPointLength(e->v);
		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
		}
	}
#endif
}

void ProcEnemyAI::ArabMeleeKessie(ProcEnemy* e, float dt)
{
	//e->p += e->v * e->moveSpeed * dt;
}

void ProcEnemyAI::ArabBurserkAI0(ProcEnemy* e, float dt)		//Spawn From Kessie
{
	if (e->v != iPointZero)
		return;
	e->v.x = -1;

	float len = iPointLength(e->v);
	if (len > 0)
	{
		len /= (e->moveSpeed * dt);
		e->v.x / len;
		e->v.y / len;
	}

	if (e->v != iPointZero)
	{
		if (e->v.x > 0)
			e->setState(WalkBurserkR);
		else if(e->v.x<0)
			e->setState(WalkBurserkL);
	}


}

void ProcEnemyAI::ArabCamelAI0(ArabCamel* e, float dt)
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp.x = map->off.x;
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

void ProcEnemyAI::TruckAI0(ProcEnemy* e, float dt)
{
}

static iPoint initPos;
void ProcEnemyAI::KessieAI(ProcEnemy* k, float dt)
{
}

void ProcEnemyAI::KessieRageAI(ProcEnemy* k, float dt)
{
	if (k->v != iPointZero)
	{
		if (initPos == iPointZero)
			initPos = k->p;

		int maxX;
		if (k->v.x > 0)
		{
			maxX = initPos.x + 70;
			if (k->p.x > maxX)
				k->v.x = -1;
		}
		else if (k->v.x < 0)
		{
			maxX = initPos.x - 70;
			if (k->p.x < maxX)
				k->v.x = 1;
		}
	}
	k->p += k->v * k->moveSpeed * dt;
}

void ProcEnemyAI::KessieDeadAI(ProcEnemy* k, float dt)
{
	k->v = { 1,0 };
	int maxY = *(map->maxY + (int)k->p.x);
	movePoint(k->p, k->p, iPointMake(k->p.x, maxY), k->moveSpeed * 0.6 * dt);

	if (k->v != iPointZero)
	{
		if (initPos == iPointZero)
			initPos = k->p;

		int maxX;
		if (k->v.x > 0)
		{
			maxX = initPos.x + 10;
			printf("%d\n", maxX);
			if (k->p.x > maxX)
				k->v.x = -1;
		}
		else if (k->v.x < 0)
		{
			maxX = initPos.x - 10;
			printf("%d\n", maxX);
			if (k->p.x < maxX)
				k->v.x = 1;
		}
	}
	k->p += k->v * k->moveSpeed * dt;	
}

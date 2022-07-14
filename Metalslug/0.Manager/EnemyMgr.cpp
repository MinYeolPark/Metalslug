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

ArabCamel* arabCamel;
Truck* truck;
Kessie* kessie;
Mosque* mosque;
Abul* abul;
int maxNum[EnemyIndexMax] =
{
	50,		//melee
	20,		//kessie melee
	//50,		//burser
	//1,		//camel
	//1,		//truck
	//1,		//kessie
	//1,		//mosque
	//1,		//abul
};
void loadProcEnemy()
{
	_enemies = new ProcEnemy * * [EnemyIndexMax];
	for (int i = 0; i < EnemyIndexMax; i++)
	{		
		_enemies[i] = new ProcEnemy * [maxNum[i]];
		if (i == IdxArMelee || i==IdxArMeleeKessie)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
#if 0
		else if (i == IdxArBurserker)
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
		else if (i == IdxMosque)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Mosque(i);
		}
		else if (i == IdxAbul)
		{
			for (int j = 0; j < maxNum[i]; j++)
				_enemies[i][j] = new Abul(i);
		}
#endif
		enemies = new ProcEnemy * [EnemyIndexMax *  maxNum[i]];
	}
	enemyCount = 0;

	//boss monster
	//arabCamel = new ArabCamel(IdxArCamel);
	//truck = new Truck(IdxTruck);
	//abul = new Abul(IdxAbul);
	//kessie = new Kessie(IdxKessie);
	//spawn pattern
	
	//addProcEnemy(IdxMosque, iPointMake(2030, 175), iPointZero);
	//addProcEnemy(IdxTruck, iPointMake(1680, 100), iPointZero);
	addProcEnemy(IdxArMelee, iPointMake(500, 50), iPointZero);
	//addProcEnemy(IdxArBurserker, iPointMake(300, 100), iPointMake(0, 0));


	//addProcEnemy(IdxArMelee, iPointMake(400, 100), iPointMake(0, 0));
	//addProcEnemy(IdxArCamel, iPointMake(300, 100), iPointMake(0, 0));
	//addProcEnemy(IdxKessie, iPointMake(3680, -50), iPointMake(0, 0));

	//addProcEnemy(IdxAbul, iPointMake(300, 100), iPointMake(0, 0));
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
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];
		if (e->isActive)
		{
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
	for (int j = 0; j < maxNum[j]; j++)
	{
		for (int i = 0; i < maxNum[j]; i++)
		{
			ProcEnemy* e = _enemies[index][i];
			if (e->isActive == false)
			{
				e->isActive = true;
				e->init(p);			//default direction = Left
				enemies[enemyCount] = e;
				enemyCount++;
				return;
			}
		}
	}
}

void ProcEnemyAI::ArabMeleeAI0(ProcEnemy* e, float dt)		//Check Player
{
	iPoint tp = e->p;
	int len = iPointLength(e->p - player->p);
	if (len < e->sight)
	{
		iPoint v = player->p - e->p;
		v /= iPointLength(v);
		e->v = v;
		
		tp = player->p;
		e->tp = tp;
	}	
}

void ProcEnemyAI::ArabMeleeAI1(ProcEnemy* e, float dt)
{

}

void ProcEnemyAI::ArabMeleeKessie(ProcEnemy* e, float dt)
{
	//e->p += e->v * e->moveSpeed * dt;
}

void ProcEnemyAI::ArabBurserkAI0(ProcEnemy* e, float dt)		//Spawn From Kessie
{
	iPoint tp = e->p;
	int len = iPointLength(e->p - player->p);
	if (len < e->sight)
	{
		iPoint v = player->p - e->p;
		v /= iPointLength(v);
		e->v = v;
		
		tp = player->p;
		e->tp = tp;
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

void ProcEnemyAI::KessieRageAI(ProcEnemy* e, float dt)
{
	Kessie* k = (Kessie*)e;
	if (!k->isDead)
	{
		if (k->v != iPointZero)
		{
			if (initPos == iPointZero)
				initPos = k->p;
			int maxX;
			int maxY;
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

			if (k->v.y > 0)
			{
				maxY = initPos.y + 5;
				if (k->p.y > maxY)
					k->v.y = -1;
			}
			else if (k->v.y < 0)
			{
				maxY = initPos.y - 5;
				if (k->p.y < maxY)
					k->v.y = 1;
			}
		}
		k->p += k->v * k->moveSpeed * dt;
	}
}

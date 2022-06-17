#pragma once
#include "iStd.h"

#include "ProcEnemy.h"
#define enemyMax 50
extern ProcEnemy** enemies;
extern int enemyCount;
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int idx, iPoint p);
void addProcEnemy(int idx, iPoint p, iPoint v, EnemyAI ai);

struct AI
{
	static void enemyAI0(ProcEnemy* e, float dt);
	static void enemyAI1(ProcEnemy* e, float dt);
	static void enemyAIBurserker(ProcEnemy* e, float dt);
};
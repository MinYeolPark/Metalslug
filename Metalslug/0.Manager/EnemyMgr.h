#pragma once
#include "iStd.h"

#include "ProcEnemy.h"
#define enemyMax 50
extern ProcEnemy** enemies;
extern int enemyCount;
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int index, iPoint p, iPoint v);

struct ProcEnemyAI
{
	static void ArabMeleeAI0(ProcEnemy* e, float dt);			//Normal
	static void ArabMeleeAI1(ProcEnemy* e, float dt);			//Kessie
	static void ArabBurserkAI0(ProcEnemy* e, float dt);
};
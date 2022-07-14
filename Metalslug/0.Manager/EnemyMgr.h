#pragma once
#include "iStd.h"

#include "ProcEnemy.h"
#include "ArabCamel.h"
#include "Kessie.h"
#define truckMax 1
#define kessieMax 1
#define mosqueMax 1
extern ProcEnemy** enemies;
extern int enemyCount;
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int index, iPoint p);

struct ProcEnemyAI
{
	static void ArabMeleeAI0(ProcEnemy* e, float dt);			//AD
	static void ArabMeleeAI1(ProcEnemy* e, float dt);			//Shuffle
	static void ArabMeleeKessie(ProcEnemy* e, float dt);			//Kessie
	static void ArabBurserkAI0(ProcEnemy* e, float dt);
	static void ArabCamelAI0(ArabCamel* e, float dt);
	static void TruckAI0(ProcEnemy* e, float dt);
	static void KessieAI(ProcEnemy* k, float dt);
	static void KessieRageAI(ProcEnemy* k, float dt);
};
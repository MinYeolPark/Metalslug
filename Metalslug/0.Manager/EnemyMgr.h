#pragma once
#include "iStd.h"

#include "ProcEnemy.h"
#include "ArabCamel.h"
#include "Kessie.h"
#define truckMax 1
#define kessieMax 1
#define mosqueMax 1
extern ProcEnemy** enemies;
extern int enemyNum;
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
ProcEnemy* addProcEnemy(int index, iPoint p);

extern int maxNum[EnemyIndexMax];
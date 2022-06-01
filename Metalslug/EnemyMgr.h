#pragma once
#include "iStd.h"

#include "ProcEnemy.h"
///////////////////////////////////////////////////
//Eneimies
///////////////////////////////////////////////////
#define enemyMax 50
extern ProcEnemy** enemies;
extern int enemyCount;
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int idx, iPoint p);

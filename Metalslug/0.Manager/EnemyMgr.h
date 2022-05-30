#pragma once

#include "ProcField.h"
#include "ProcBullets.h"

#define enemyMax 50
extern ProcEnemy** enemies;			//to access enemy
extern int enemyCount;

void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int idx, iPoint p);

void addBoss(int idx, iPoint p);


struct EnemyImageInfo
{
	const char* path;
	int num;
	float s;
	iPoint p;

	float aniDt;
	int repeatNum;
	cbAnimation cbAni;
};

iImage** createImgChar(EnemyImageInfo* imageInfo, void* c);
iImage** createImgCharReverse(EnemyImageInfo* imageInfo, void* c);

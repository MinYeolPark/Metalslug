#pragma once

#include "iStd.h"

#include "Mosque.h"
#include "ProcPlayer.h"
#include "ProcEnemy.h"
#include "ProcBullets.h"

extern int bulletNum;
extern ProcBullets** bullets;
#define bulletMax 50
void addBullet(ProcObject* parent, int index, float degree, int fpNum = 0);
//void addBullet(ProcPlayer* player, int index, float degree);
//void addBullet(ProcEnemy* enemy, int index, float degree);
//void addBullet(Mosque* enemy, int which, int index, float degree);
void loadProcBullets();
void freeProcBullets();
void drawProcBullets(float dt, iPoint off);

struct ProcBulletsPattern
{
	static void patternHandgun(ProcBullets* b, float dt);
	static void patternHeavyMachinegun(ProcBullets* b, float dt);
	static void patternBomb(ProcBullets* b, float dt);
	static void patternMelee(ProcBullets* b, float dt);
	static void patternMeleeEnd(ProcBullets* b, float dt);
	static void patternMosque(ProcBullets* b, float dt);
	static void patternMosqueTrace(ProcBullets* b, float dt);
};
#pragma once

#include "iStd.h"

#include "ProcPlayer.h"
#include "ProcEnemy.h"
#include "ProcBullets.h"

extern int bulletNum;
extern ProcBullets** bullets;
#define bulletMax 50
void addBullet(ProcPlayer* player, int idx, float degree);
void loadProcBullets();
void freeProcBullets();
void drawProcBullets(float dt, iPoint off);

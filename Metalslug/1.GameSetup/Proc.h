#pragma once

#include "iStd.h"

#include "ImgMgr.h"

#include "EnemyMgr.h"

#include "ProcField.h"
#include "ProcPlayer.h"
#include "ProcBullets.h"
#include "ProcEnemy.h"

///////////////////////////////////////////////////
//Proc
///////////////////////////////////////////////////
void loadProc();
void freeProc();
void drawProc(float dt);
bool keyProc(iKeyState stat, iPoint p);

///////////////////////////////////////////////////
//Field
///////////////////////////////////////////////////
void loadProcField();
void freeProcField();
iPoint drawProcField(float dt);

///////////////////////////////////////////////////
//Player
///////////////////////////////////////////////////
extern ProcPlayer* player;
void loadProcPlayer();
void freeProcPlayer();
void drawProcPlayer(float dt, iPoint off);

///////////////////////////////////////////////////
//Bullets
///////////////////////////////////////////////////
extern int bulletNum;
extern ProcBullets** bullets;
#define bulletMax 50
void addBullet(ProcObject* parent, int idx, int dir);
void loadProcBullets();
void freeProcBullets();
void drawProcBullets(float dt, iPoint off);



#pragma once

#include "iStd.h"

#include "ImgMgr.h"
#include "ProcField.h"
#include "ProcPlayer.h"
#include "ProcBullets.h"

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

///////////////////////////////////////////////////
//UI
///////////////////////////////////////////////////
void loadUI();
void freeUI();
void drawUI(float dt, iPoint off);
void addUI(iPoint p, int num);

///////////////////////////////////////////////////
//Eneimies
///////////////////////////////////////////////////
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int idx, iPoint p);

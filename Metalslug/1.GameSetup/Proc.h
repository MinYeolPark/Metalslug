#pragma once

#include "iStd.h"

#include "ImgMgr.h"

#include "EnemyMgr.h"
#include "EffectMgr.h"

#include "ProcField.h"
#include "ProcPlayer.h"
#include "ProcBullets.h"
#include "ProcEnemy.h"

#include "Mosque.h"
//Components

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
extern int stage;
///////////////////////////////////////////////////
//Player
///////////////////////////////////////////////////
#define _respawnDt 1
extern ProcPlayer* player;
void loadProcPlayer();
void freeProcPlayer();
void drawProcPlayer(float dt, iPoint off);




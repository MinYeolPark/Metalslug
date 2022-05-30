#pragma once

#include "iStd.h"

#include "ImgMgr.h"
#include "ProcField.h"
#include "ProcPlayer.h"
#include "ProcBullets.h"

#if 0
struct ImageInfo
{
	PlayerIndex idx;
	const char* path;
	int spriteType;
	int num;
	float s;
	iPoint p;

	float aniDt;
	int repeatNum;
	cbAnimation cbAni;
	PlayerBehave pb0, pb1;
};
#endif

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
//Eneimies
///////////////////////////////////////////////////
void loadProcEnemy();
void freeProcEnemy();
void drawProcEnemy(float dt, iPoint off);
void addProcEnemy(int idx, iPoint p);
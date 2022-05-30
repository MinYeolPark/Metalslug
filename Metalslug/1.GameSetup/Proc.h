#pragma once

#include "iStd.h"

#include "ProcField.h"
#include "ProcBullets.h"
struct ImageInfo
{
	const char* path;
	int num;
	float s;
	iPoint p;

	float aniDt;
	int repeatNum;
	cbAnimation cbAni;
};

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
//Bullets
///////////////////////////////////////////////////
extern int bulletNum;
extern ProcBullets** bullets;
#define bulletMax 50
void addBullet(ProcObject* parent, int idx, int dir);
void loadProcBullets();
void freeProcBullets();
void drawProcBullets(float dt, iPoint off);
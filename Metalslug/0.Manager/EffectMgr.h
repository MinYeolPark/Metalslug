#pragma once

#include "iStd.h"

#include "ProcPlayer.h"
#define effectMax 100
void loadProcEffect();
void freeProcEffect();
void drawProcEffect(float dt, iPoint off);
void addProcEffect(int index, iPoint p);
void addProcEffect(int index, iPoint p, float spawnDt);

enum EffectIndex
{
	EffectHand,
	EffectHeavy,
	EffectExplosion,

	EffectMoskTrail,
	EffectKessieBlast,
	EffectKessieBlastRage,
	EffectIndexMax,
};

struct ProcEffect
{
	ProcEffect();
	virtual ~ProcEffect();

	iImage** imgs;	
	iImage* imgCurr;

	iPoint p;
	iSize s;

	EffectIndex index;
	bool isActive;
	float delta, _delta;
	float spawnDt, _spawnDt;

	void initEffect(int idx, iPoint p);
	void initEffect(int idx, iPoint p, float spawnDt);
	void updateEffect(float dt);
	bool drawEffect(float dt, iPoint off);
	void freeEffect();
		
	static void cbAniEffect(void* parm);
};
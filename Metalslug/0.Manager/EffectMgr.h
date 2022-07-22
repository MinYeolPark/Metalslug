#pragma once

#include "iStd.h"

#include "ProcPlayer.h"
#define effectMax 100
void loadProcEffect();
void freeProcEffect();
void drawProcEffect(float dt, iPoint off);
void addProcEffect(ProcObject* parent, int index, iPoint p, int id = 0);
void addProcEffect(ProcObject* parent, int index, iPoint p, float spawnDt);

enum EffectIndex
{
	EffectHand,
	EffectHeavy,
	EffectBomb,

	EffectBlood,
	

	//
	EffectExplosionL,
	EffectExplosionM,
	EffectMoskTrail,
	EffectKessieBlastStart,
	EffectKessieBlast,
	EffectKessieBlastEnd,
	EffectKessieFanFire,
	EffectKessieExplosion,

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

	ProcObject* parent;
	int id;		//compare position
	int index;
	bool isActive;
	float delta, _delta;
	float spawnDt, _spawnDt;

	void init(ProcObject* parent, int idx, iPoint p);
	void init(ProcObject* parent, int idx, iPoint p, float spawnDt);
	void update(float dt);
	bool draw(float dt, iPoint off);
	void free();
		
	static void cbAniEffect(void* parm);
};

extern ProcEffect** effect;

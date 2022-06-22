#pragma once

#include "ProcObject.h"
#include "iStd.h"
#define npcMax 20
void loadProcNpcs();
void freeProcNpcs();
void drawProcNpcs(float dt, iPoint off);
void addNPC(int index, iPoint p);
void addNPC(int index, iPoint p, int itemIndex);

enum NpcIndex
{
	NpcIchimonji,

	NpcIndexMax,
};

enum NpcBehave
{
	IdleNpcL = 0,
	IdleNpcR,

	ReleaseNpcL,
	ReleaseNpcR,

	WalkNpcL,
	WalkNpcR,

	AddItemNpcL,
	AddItemNpcR,

	SaluteNpcL,
	SaluteNpcR,

	EscapeNpcL,
	EscapeNpcR,

	NpcBehaveMax,
};
class ProcNpc
{
public:
	ProcNpc();
	ProcNpc(int idx) { ; }
	virtual ~ProcNpc();

public:	
	iPoint p;
	iPoint v;
	iSize s;

	NpcIndex index;
	NpcBehave state;
	
	bool isActive;
	bool complete;
	int itemIndex;

	float up;
	float down;
	bool fall;
	float moveSpeed;

	virtual iRect collider() = 0;
	virtual void spawnItem() = 0;
	virtual void setState(NpcBehave newState) { state = newState; }

	virtual void set(int index, iPoint p) = 0;
	virtual void set(int idex, iPoint p, int itemIndex) = 0;
	virtual void update(float dt) = 0;
	virtual void fixedUpdate(float dt) = 0;
	virtual bool draw(float dt, iPoint off) = 0;		

public:
	static void cbAniSpawnItem(void* n);
	static void cbAniSalute(void* n);
	static void cbAniNpcOut(void* n);
};


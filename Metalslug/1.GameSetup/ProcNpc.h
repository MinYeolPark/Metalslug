#pragma once

#include "iStd.h"
#include "ProcObject.h"
#define npcMax 20
void loadProcNpcs();
void freeProcNpcs();
void drawProcNpcs(float dt, iPoint off);
void addProcNPC(int index, iPoint p);
void addProcNPC(int index, iPoint p, int itemIndex);
//extern ProcNpc** npcs;
//extern int npcNum;
enum NpcIndex
{
	NpcIchimondi,

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
class ProcNpc:
	public ProcObject
{
public:
	ProcNpc(int index);
	virtual ~ProcNpc();
public:	
	NpcBehave state;

	bool complete;
	bool escape;
	int itemIndex;

	float up;
	float down;
	bool fall;
	float moveSpeed;
	float aiDt, _aiDt;

	iPoint tp;
public:
	virtual void getDamage(float damage);
	virtual void spawnItem() = 0;
	virtual void setState(NpcBehave newState) { state = newState; }

	virtual void init(int idex, iPoint p, int itemIndex);
	virtual void fixedUpdate(float dt);
};
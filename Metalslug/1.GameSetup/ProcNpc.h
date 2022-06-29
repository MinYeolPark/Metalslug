#pragma once

#include "iStd.h"
#include "ProcObject.h"
#define npcMax 20
void loadProcNpcs();
void freeProcNpcs();
void drawProcNpcs(float dt, iPoint off);
void addProcNPC(int index, iPoint p);
void addProcNPC(int index, iPoint p, int itemIndex);

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
class ProcNpc;
typedef void(*NpcAI)(ProcNpc* n, float dt);
class ProcNpc:
	public ProcObject
{
public:
	ProcNpc(int index);
	virtual ~ProcNpc();
public:	
	NpcBehave state;
	NpcAI ai;

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
	virtual void getDamage(float damage, Collider* c);
	virtual void spawnItem() = 0;
	virtual void setState(NpcBehave newState) { state = newState; }
	virtual NpcBehave getState() { return state; }

	virtual void init(int idex, iPoint p, int itemIndex);
	virtual void fixedUpdate(float dt);
};

struct ProcNpcAI
{
	static void IchimondiAI0(ProcNpc* n, float dt)
	{

	}
	static void IchimondiAI1(ProcNpc* n, float dt)
	{

	}
};
#pragma once
#include "ProcEnemy.h"

enum KessieState
{
	KessieIdle,
	KessieRage,

	KessieStateMax,
};
enum KessieBlastState
{
	BlastIdle,
	BlastStart,
	Blasting,
	BlastEnd,

	KessieBlastStateMax,
};
enum KessieBehave
{
	KessieBase100 = 0,
	KessieBase80,
	KessieBaseRight60,
	KessieBaseLeft60,
	KessieBaseBoth60,
	KessieBase40,
	KessieBase20,
	KessieBase0,

	KessieCraterLeft100,
	KessieCraterRight100,

	KessieCraterLeft70,
	KessieCraterRight70,

	KessieCraterLeftDead,
	KessieCraterRightDead,

	KessieFanLeft,
	KessieFanRight,

	KessieFanLeftRage,
	KessieFanRightRage,	

	KessieBlastIdle,
	KessieBlast,
	KessieBlastEnd,
	KessieBlastRage,

	KessieHead,

	KessieBehaveMax,
};
class Kessie  
	:public ProcEnemy
{
public:
	Kessie(int index);
	virtual ~Kessie();

public:
	iImage* imgBase[8];
	iImage* imgHead;
	iImage* imgLeftCrater[3];
	iImage* imgRightCrater[3];
	iImage* imgLeftFan[2];			//Idle, Rage
	iImage* imgRightFan[2];			//Idle, Rage
	iImage* imgLeftBlast[4];
	iImage* imgRightBlast[4];

public:
	int baseState;
	int headState;
	int craterState[2];
	int fanState;
	int blastState[2];

	bool isHeadOpen;

	iRect* atkRect[2];
	float rageDt, _rageDt;	
	float aiDt, _aiDt;
	float dmgDt, _dmgDt;
	int hp[3], _hp;
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();

public:
	virtual void getDamage(float damage);
};


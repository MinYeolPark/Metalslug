#pragma once
#include "ProcEnemy.h"

enum KessieState
{
	KessieIdle,
	KessieRage,

	KessieStateMax,
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

	KessieBlast,
	KessieBlastRage,

	KessieHead,

	KessieBehaveMax,
};
class Kessie  
	:public ProcEnemy
{
public:
	Kessie(int idx);
	virtual ~Kessie();

public:
	iImage* imgBase[8];
	iImage* imgHead;
	iImage* imgLeftCrater[3];
	iImage* imgRightCrater[3];
	iImage* imgLeftFan[2];			//Idle, Rage
	iImage* imgRightFan[2];			//Idle, Rage
	iImage* imgLeftBlast[3];
	iImage* imgRightBlast[3];

public:
	int baseState;
	int headState;
	int craterState;
	int fanState;
	int blastState;
	bool isAppear;					//appear state
	bool isHeadOpen;

	float effDt, _effDt;	
	float aiDt, _aiDt;
	int hpLeft, hpRight, _hp;

	Collider* leftCollider;
	Collider* rightCollider;
	Collider* attkCollider;

	virtual int getFrame() { return NULL; }
	virtual bool dead();
public:
	virtual void getDamage(float damage, Collider* c);
	virtual void setState(int newState);

public:
	void update(float dt);
	void fixedUpdate(float dt);
	bool draw(float dt, iPoint off);
	void free();
};


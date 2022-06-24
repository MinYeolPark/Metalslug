#pragma once
#include "ProcEnemy.h"

enum KessieBehave
{
	KessieBase100 = 0,
	KessieBase80,
	KessieBaseRight60,
	KessieBaseLeft60,
	KessieBaseBoth60,
	KessieBase40,
	KessieBase20,

	KessieCraterLeft100,
	KessieCraterLeft70,
	KessieCraterLeftDead,
	KessieCraterRight100,
	KessieCraterRight70,
	KessieCraterRightDead,

	KessieFanLeft,
	KessieFanLeftRange,
	KessieFanRight,
	KessieFanRightRange,
	
	KessieHead,
	KessieBlast,
	KessieBlastRage,

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
	bool isAppear;					//appear state
	bool isHeadOpen;

	float effDt, _effDt;	
	float aiDt, _aiDt;
	int hpLeft, hpRight, _hp;

	Collider* leftCollider;
	Collider* rightCollider;
	Collider* attkCollider;
public:
	virtual void getDamage(float damage);
	virtual void setState(int newState);
	virtual bool dead();

public:
	void update(float dt);
	void fixedUpdate(float dt);
	bool draw(float dt, iPoint off);
	void free();
};


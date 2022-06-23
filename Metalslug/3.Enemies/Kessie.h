#pragma once
#include "ProcEnemy.h"

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

	Collider* attkCollider;
public:
	virtual bool dead();

public:
	void update(float dt);
	bool draw(float dt, iPoint off);
	void free();
};


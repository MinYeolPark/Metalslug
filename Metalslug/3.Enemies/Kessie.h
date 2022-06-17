#pragma once

#include "iStd.h"

class Kessie
{
public:
	Kessie();
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
	iPoint p;

	bool isActive;
	bool isHeadOpen;

	float effDt, _effDt;	
	float aiDt, _aiDt;
	int hpLeft, hpRight, _hp;

public:
	iRect attkCollider()
	{

	}
	iRect collider()
	{

	}
public:
	void init();
	void update(float dt);
	void fixedUpdate(float dt);
	bool draw(float dt, iPoint off);
	void free();
};


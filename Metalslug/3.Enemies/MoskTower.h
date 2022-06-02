#pragma once
#include "ProcEnemy.h"

enum MidBossBehave
{
	mtIdle=0,
	mtDead,
	MidBossBehaveMax,
};

class MoskTower
{
public:
	MoskTower();
	~MoskTower();
public:
	iImage* imgBase[2];
	iImage* imgAdd[2];

	//clone imgs
	iImage* imgDoor[3];
	iImage* imgMis[3];	
	iImage* imgTrail[3];
	iImage* imgCurtain[3][2];
	iImage* imgSoldier[3][2];

	//dif imgs
	iImage* imgTower[3][2];

	MidBossBehave sdState[3];
	MidBossBehave ctState[3];
	MidBossBehave twState[3];
	MidBossBehave state;

	iPoint p;
	iPoint towerP[3];
	bool doorOpen[3];
	iPoint doorP[3];
	iPoint curP[3];
	iPoint misP[3];

	iSize s;

	bool isActive;

	float hp[3], _hp[3];
	float createDt[3], _createDt;
	float rate[3], _rate;

public:
	void initObj();
	void updateObj(float dt);
	void fixedUpdate(float dt) ;	
	void drawObj(float dt, iPoint off);
	void freeeObj() ;

	//Behave
public:
	void fire(float dt);
	void dead();
	//Ani callback
public:
	static void cbAniDoorOpen(void* parm);
	static void cbAniCurOpen(void* parm);
	static void cbAniAddOut(void* parm);

public:
	void setDoorPos(iPoint p)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == 2)
				doorP[i].x = towerP[i].x + 3;
			else
				doorP[i].x = towerP[i].x - 3;
			doorP[i].y = towerP[i].y - 140;			
		}
	}
};
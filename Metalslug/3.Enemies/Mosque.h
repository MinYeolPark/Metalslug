#pragma once

#include "iStd.h"

enum MosqueBaseBehave
{
	MosqueIdle = 0,
	MosqueDead,

	MosqueBehaveMax,
};

class Mosque
{
public:
	Mosque();
	~Mosque();
public:
	iImage* imgBase[2];
	iImage* imgDoor[2];	
	
	iImage* imgLeftTower[2];
	iImage* imgMidTower[2];
	iImage* imgRightTower[2];

	iImage* imgLeftCurtain[2];
	iImage* imgMidCurtain[2];
	iImage* imgRightCurtain[2];

	iImage* imgShutter[3][2];
	iImage* imgSoldier[3][2];	

	MosqueBaseBehave baseState;
	MosqueBaseBehave doorState;
	MosqueBaseBehave towerState[3];
	MosqueBaseBehave shutterState[3];
	MosqueBaseBehave curtainState[3];
	MosqueBaseBehave soldierState[3];

	iPoint doorP[2];
	iPoint towerP[3];
	iPoint curtainP[3];
	iPoint shutterP[3];
	bool isShutterOpen[3];
	iPoint soldierP[3];

	iPoint p;
	iPoint tp;
	iSize s;

	bool isActive;

	float hp[3], _hp;
	float rate[3], _rate;

public:
	void initObj();
	void updateObj(float dt);
	void fixedUpdate(float dt) ;	
	bool drawObj(float dt, iPoint off);
	void freeeObj() ;

	//Behave
public:
	void fire(float dt);
	void dead();
	//Ani callback
public:
	static void cbAniShutterOpen(void* parm);
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
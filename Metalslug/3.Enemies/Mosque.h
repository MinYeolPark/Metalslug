#pragma once

#include "iStd.h"

#include "ProcEnemy.h"
enum MosqueBaseBehave
{
	MosqueIdle = 0,
	MosqueDead,

	MosqueBehaveMax,
};

class Mosque:
	public ProcEnemy
{
public:
	Mosque(int index);
	~Mosque();
public:
	iImage* imgBase[2];
	iImage* imgDoor[2];	
	
	iImage* imgTower[3][2];
	iImage* imgCurtain[3][2];

	iImage* imgShutter[3][2];
	iImage* imgSoldier[3][2];	

	int baseState;
	int doorState;
	int towerState[3];
	int shutterState[3];
	int curtainState[3];
	int soldierState[3];

	iPoint doorP[2];
	iPoint towerP[3];
	iPoint curtainP[3];
	iPoint shutterP[3];
	iPoint soldierP[3];

	float aiDt[3], _aiDt;
	float hp[3], _hp;		//0 : left, 1: mid, 2: right
	
	virtual iPoint getFirePoint();
	virtual int getFrame() { return NULL; }
	virtual bool dead();
public:
	virtual void getDamage(float damage, Collider* c);
	virtual void setState(int newState);

public:
	virtual void init(int index, iPoint p, iPoint v);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();

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
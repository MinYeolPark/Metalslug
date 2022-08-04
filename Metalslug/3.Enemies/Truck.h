#pragma once
#include "ProcEnemy.h"

enum TruckBehave
{
	IdleTruckL = 0,
	IdleTruckR,

	MoveTruckL,
	MoveTruckR,

	DeployTruckL,
	DeployTruckR,

	DeadTruckL,
	DeadTruckR,

	TruckBehaveMax,
};
class Truck:
	public ProcEnemy
{
public:
	Truck(int index);
	~Truck();

public:
	iImage** imgs;
	iImage* imgCurr;

	float dmgDt, _dmgDt;
public:
	virtual void getDamage(float damage);
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};


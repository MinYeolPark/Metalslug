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

	virtual int getFrame();
	virtual bool dead();
public:
	virtual void getDamage(float damage, Collider* c);
	virtual void setState(int newState);
public:
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};


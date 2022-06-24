#pragma once
#include "ProcEnemy.h"
enum ArabBurserkBehave
{
	IdleBurserkL = 0,
	IdleBurserkR,
	WalkBurserkL,
	WalkBurserkR,
	DeadBurserkL,
	DeadBurserkR,
	
	BurserkBehaveMax,
};
class ArabBurserker :
	public ProcEnemy
{
public:
	ArabBurserker(int index);
	~ArabBurserker();

public:
	iImage** imgs;
	iImage* imgCurr;

	virtual bool dead();
public:
	virtual void getDamage(float damage);
	virtual void setState(int newState);
public:
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};
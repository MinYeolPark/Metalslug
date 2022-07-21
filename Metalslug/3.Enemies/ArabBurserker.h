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
	iImage** topImgs;
public:
	virtual void getDamage(float damage);
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};
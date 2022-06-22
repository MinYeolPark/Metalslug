#pragma once
#include "ProcEnemy.h"

class ArabMelee :
    public ProcEnemy
{	
public:
	ArabMelee(int idx);
	~ArabMelee();

public:
	iImage** imgs;
	iImage* imgCurr;

	int idx;
public:
	virtual iRect collider() { return iRectMake(p.x - 20, p.y - 45, 40, 45); }
public:
	virtual void setState(EnemyBehave newState);
public:
	virtual void initObj();
	virtual void initObj(iPoint v);
	virtual void updateObj(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool drawObj(float dt, iPoint off);
	virtual void freeObj();
};

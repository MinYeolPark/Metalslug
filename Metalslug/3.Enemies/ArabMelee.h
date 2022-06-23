#pragma once
#include "ProcEnemy.h"

class ArabMelee :
    public ProcEnemy
{	
public:
	ArabMelee(int index);
	~ArabMelee();

public:
	iImage** imgs;
	iImage* imgCurr;	

	//Ground unit
	float up;
	float down;
	bool fall;

	virtual void dead();
public:
	virtual void setState(EnemyBehave newState);
public:
	//virtual iRect collider();
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};

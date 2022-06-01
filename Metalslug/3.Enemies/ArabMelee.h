#pragma once
#include "ProcEnemy.h"

class ArabMelee :
    public ProcEnemy
{
	enum MeleeBehave
	{
		IdleL,
		IdleR,

		WalkL,
		WalkR,

		MeleeBehaveMax,
	};
public:
	ArabMelee(int idx);
	~ArabMelee();

public:
	iImage** imgs;
	iImage* imgCurr;

	EnemyIndex idx;
	MeleeBehave state;

public:
	float speed;
	float up;
	float down;
	bool fall;

public:
	iRect collider();
public:
	virtual void initObj();
	virtual void updateObj(float dt);
	virtual void fixedUpdate(float dt);
	virtual void drawObj(float dt, iPoint off);
	virtual void freeObj();
};

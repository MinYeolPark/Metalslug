#pragma once
#include "ProcEnemy.h"
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
	virtual void setState(EnemyBehave newState);
public:
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};
#pragma once
#include "ProcEnemy.h"
class ArabBurserker :
    public ProcEnemy
{
public:
	ArabBurserker(int idx);
	~ArabBurserker();

public:
	virtual void initObj();
	virtual void updateObj(float dt);
	virtual void fixedUpdate(float dt);
	virtual void renderObj(float dt, iPoint off);
	virtual void releaseObj();
};

void cleanArabBurserker();

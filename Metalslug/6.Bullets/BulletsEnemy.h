#pragma once
#include "ProcBullets.h"
class BulletsEnemy :
    public ProcBullets
{
public:
	BulletsEnemy(int index);
	virtual ~BulletsEnemy();

public:
	virtual void init(ProcObject* parent, int index, float degree, int fpNum);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
};


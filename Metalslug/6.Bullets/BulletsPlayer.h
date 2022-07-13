#pragma once
#include "ProcBullets.h"

class BulletsPlayer :
    public ProcBullets
{
public:
    BulletsPlayer(int index);
    virtual ~BulletsPlayer();
public:
    virtual void init(ProcObject* parent, int index, float degree, int fpNum);
    virtual void update(float dt);
};


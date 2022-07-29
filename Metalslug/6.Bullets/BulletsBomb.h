#pragma once
#include "ProcBullets.h"

class BulletsBomb :
    public ProcBullets
{
public:
    BulletsBomb(int index);
    virtual ~BulletsBomb();


public:
    virtual void init(ProcObject* parent, int index, float degree, int fpNum = 1);
    virtual void update(float dt);
};


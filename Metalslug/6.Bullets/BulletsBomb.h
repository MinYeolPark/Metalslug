#pragma once
#include "ProcBullets.h"

class BulletsBomb :
    public ProcBullets
{
public:
    BulletsBomb(int index);
    virtual ~BulletsBomb();

public:
    iPoint rp;
    float wave;
    float d, _d;				//curDistance , leftDistance
public:
    virtual void init(ProcObject* parent, int index, float degree, int fpNum = 1);
    virtual void update(float dt);
    virtual void fixedUpdate(float dt);
    virtual bool draw(float dt, iPoint off);
};


#pragma once
#include "ProcBullets.h"
class BulletsMosque:
	public ProcBullets
{
public:
	BulletsMosque(int index);
	virtual ~BulletsMosque();
public:
	iPoint rp;					//real Position

	int parentIndex;
	int hp, _hp;
	float wave;
	float d, _d;				//curDistance , leftDistance

	virtual void init(ProcObject* parent, int index, float degree, int fpNum);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void getDamage(float damage);
};


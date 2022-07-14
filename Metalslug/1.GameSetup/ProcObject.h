#pragma once

#include "iStd.h"

#include "ProcField.h"
enum ObjLayer
{
	LayerDefault = 0,
	LayerPlayer,
	LayerEnemy,	
	LayerMosque,
	LayerKessie,

	LayerBullet,
	LayerFieldObj,

	ObjLayerMax = 16
};

class Collider;
class ProcObject
{
public:
	ProcObject();
	virtual ~ProcObject();
public:
	ObjLayer layer;

	iPoint p;					//position
	iPoint tp;					//targetPosition
	iPoint v;					//direction
	iSize s;					//size

	float degree;
	float alpha;
	float alphaDt, _alphaDt;

	int index;
	bool isDead;
	bool isActive;
	bool isRight;

	int hp, _hp;
	int score;
public:
	virtual void update(float dt) = 0;				//With control physics
	virtual bool draw(float dt, iPoint off) = 0;

public:
	virtual int getScore();
	virtual void addScore(int addScore);
	virtual void getDamage(float damage);
	virtual void getDamage(float damage, Collider* c);
};
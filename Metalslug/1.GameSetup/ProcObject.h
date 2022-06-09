#pragma once

#include "iStd.h"

enum ObjLayer
{
	Default = 0,
	Player,
	GroundEnemy,
	HoverEnemy,
	Bullet,
	ObjLayerMax = 16
};

class ProcObject
{
public:
	ProcObject();
	ProcObject(int idx);
	virtual ~ProcObject();

public:
	ObjLayer layer;

public:
	iPoint p;					//position
	iSize s;					//size

	bool isActive;

public:
	virtual void initObj() = 0;
	virtual void updateObj(float dt) = 0;
	virtual void fixedUpdate(float dt) = 0;
	virtual bool drawObj(float dt, iPoint off) = 0;
	virtual void freeObj() = 0;
};

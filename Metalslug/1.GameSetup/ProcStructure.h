#pragma once
#include "iStd.h"
#include "ProcObject.h"

enum StructureIndex
{
	AppleStair,
	Wall,
	FootStep,
	StructureIndexMax,
};
class ProcStructure:
	public ProcObject
{
public:
	ProcStructure();
	virtual ~ProcStructure();

public:
	iImage** imgs;
	iImage* imgCurr;

	bool isAppear;
public:
	virtual void init(int index, iPoint p);
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
public:
	virtual void getDamage(float damage, Collider* c);
};

void loadStructure();
void freeStructure();
void drawStructure(float dt, iPoint off);
void addStructure(int index, iPoint p);



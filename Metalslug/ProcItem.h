#pragma once

#include "iStd.h"

#define itemMax 10
void loadProcItem();
void freeProcItem();
void drawProcItem(float dt, iPoint off);
void addProcItem(int index, iPoint p);

enum ItemIndex
{	
	ItemHandGun = 0,
	ItemHeavyMachineGun,	


	ItemBomb,

	ItemIndexMax,
};

struct ProcItem
{
	ProcItem();
	virtual ~ProcItem();

	iImage** imgs;
	iImage* imgCurr;

	iPoint p;
	iSize s;

	ItemIndex index;
	bool isActive;

	iRect collider();
	virtual void init(int index, iPoint p);
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};
//extern ProcItem** item;
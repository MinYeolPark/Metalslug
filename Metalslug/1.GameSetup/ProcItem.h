#pragma once

#include "ProcObject.h"

#define itemMax 10
void loadProcItem();
void freeProcItem();
void drawProcItem(float dt, iPoint off);
void addProcItem(int index, iPoint p);

enum ItemIndex		//match with gun
{	
	ItemHandGun=0,
	ItemHeavyMachineGun,
	ItemBomb,
	ItemIndexMax,
};

class ProcItem : 
	public ProcObject
{
public:
	ProcItem(int idx);
	virtual ~ProcItem();

	iImage** imgs;
	iImage* imgCurr;

	virtual void init(int index, iPoint p);
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};

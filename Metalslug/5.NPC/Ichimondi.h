#pragma once
#include "ProcNpc.h"

class Ichimondi:
	public ProcNpc
{
public:
	Ichimondi(int idx);
	virtual ~Ichimondi();
public:
	iImage** imgs;
	iImage* imgCurr;
public:
	iRect collider();
	void spawnItem();
	void setState(NpcBehave newState);

	virtual void set(int index, iPoint p) override;
	virtual void set(int idex, iPoint p, int itemIndex) override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float dt) override;
	virtual bool draw(float dt, iPoint off) override;
};


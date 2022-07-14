#pragma once

#include "iStd.h"

#include "ProcObject.h"
class Collider
{
public:
	Collider();
	virtual ~Collider();

	iPoint p;
	iSize s;
	float degree;
	float alpha;

	ProcObject* parent;

	int colID;
	bool isActive;
	bool isTrigger;
	bool damageable;

	void init(ProcObject* parent, iSize s);
	void update(float dt);
	bool draw(float dt, iPoint off);
	void setPosition(iPoint p);
	void setSize(iSize s);
	iRect getCollider();
	void enable() { isActive = true; }
	void disable() { isActive = false; }
};

extern iArray* procColliders;
void loadProcColliders();
void freeProcColliders();
void drawProcColliders(float dt, iPoint off);
void addColliders(ProcObject* parent, iSize s);

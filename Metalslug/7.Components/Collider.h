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

	void init(ProcObject* parent, iPoint p, iSize s);
	void update(float dt);
	bool draw(float dt, iPoint off);
	void setPosition(iPoint p);
	void setSize(iSize s);
	iRect getCollider();
	void enable() { isActive = true; }
	void disable() { isActive = false; }
};
#define colliderMax 50

extern iArray* procColliders;
extern Collider** colliders;
extern int colNum;
void loadProcColliders();
void freeProcColliders();
void drawProcColliders(float dt, iPoint off);
Collider* addColliders(ProcObject* parent, iPoint p, iSize s);

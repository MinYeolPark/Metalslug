#include "ProcObject.h"

#include "ProcField.h"

void freeArrCollider(void* data)
{
	Collider* c = (Collider*)data;
	c->disable();
}
//iArray* objects = new iArray(freeArrCollider);
iArray* objects = new iArray();
ProcObject::ProcObject()
{
	layer = ObjLayer::LayerDefault;

	p = iPointZero;
	v = iPointZero;
	s = iSizeZero;

	index = 0;
	isActive = false;
	isRight = false;
	hp=0, _hp = 0;
	score = 0;

	colNum = 0;	
	for (int i = 0; i < 3; i++)
		colliders[i] = new Collider();
}

ProcObject::~ProcObject()
{
}

int ProcObject::getScore()
{
	return score;
}

void ProcObject::init(iPoint p)
{
	this->p = p;
}

void ProcObject::init(iPoint p, iPoint v)
{
	this->p = p;
	this->v = v;
}

void ProcObject::init(int index, iPoint p)
{
}

void ProcObject::init(int index, iPoint p, iPoint v)
{
	this->index = index;
	this->p = p;
	this->v = v;
}

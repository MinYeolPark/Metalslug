#include "ProcObject.h"

#include "ProcField.h"

void freeProcObject(void* data)
{
	ProcObject* po = (ProcObject*)data;
	po->isActive = false;
}

iArray* objects = new iArray(freeProcObject);
ProcObject::ProcObject()
{
	layer = ObjLayer::LayerDefault;

	p = iPointZero;
	v = iPointZero;
	s = iSizeZero;

	index = 0;
	isActive = false;
	hp=0, _hp = 0;
	score = 0;
	collider = new Collider();
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

void ProcObject::init(int index, iPoint p, iPoint v)
{
	this->index = index;
	this->p = p;
	this->v = v;
}

void cb(void* data)
{
	ProcObject* obj = (ProcObject*)data;
	delete obj;
}

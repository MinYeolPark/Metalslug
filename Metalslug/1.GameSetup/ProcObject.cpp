#include "ProcObject.h"

#include "ProcField.h"
ProcObject::ProcObject()
{
	layer = ObjLayer::Default;

	p=iPointZero;
	s=iSizeZero;

	isActive = false;	
}

ProcObject::ProcObject(int idx)
{
	layer = ObjLayer::Default;

	p = iPointZero;
	s = iSizeZero;

	isActive = false;
}

ProcObject::~ProcObject()
{
}

void ProcObject::fixedUpdate(float dt)
{
	if (containPoint(p, iRectMake(this->p.x, this->p.y,
		devSize.width + bg->off.x + 100, devSize.height + bg->off.y + 100)) == false)
		this->isActive = false;
}

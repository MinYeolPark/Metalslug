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


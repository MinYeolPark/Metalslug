#include "ProcObject.h"

#include "ProcField.h"

ProcObject::ProcObject()
{
	layer = ObjLayer::LayerDefault;

	p = iPointZero;
	tp = p;
	v = iPointZero;
	s = iSizeZero;

	degree = 0.0f;
	alpha = 1.0f;

	index = 0;
	isDead = false;
	isActive = false;
	isRight = false;

	hp=0, _hp = 0;
	score = 0;
}

ProcObject::~ProcObject()
{
}

int ProcObject::getScore()
{
	return score;
}

void ProcObject::addScore(int addScore)
{
	score += addScore;
}

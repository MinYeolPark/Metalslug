#include "ProcEnemy.h"

ProcEnemy::ProcEnemy(int idx)
{
	p = iPointZero;
	s = iSizeZero;

	isActive = false;

	this->idx = (EnemyIndex)idx;
	hp = 0;
	dmg =0;

	up = 0;
	down = 0;
	fall = true;

	sight = 0;
	attkRange = 0;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;
	tp = iPointZero;
}

ProcEnemy::~ProcEnemy()
{
}

iRect ProcEnemy::collider()
{
	return iRect(p.x - 30, p.y- 30, 100, 100);
}

void ProcEnemy::updateAi(ProcEnemy* e, float dt)
{
}

void ProcEnemy::methodDead(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	e->isActive = false;
}

void ProcEnemy::methodAttack(void* parm)
{

}

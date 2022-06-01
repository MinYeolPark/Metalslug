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

void ProcEnemy::updateAi(ProcEnemy* e, float dt)
{
}

void ProcEnemy::methodDead(void* parm)
{
}

void ProcEnemy::methodAttack(void* parm)
{
}

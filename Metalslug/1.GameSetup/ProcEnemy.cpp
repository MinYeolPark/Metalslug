#include "ProcEnemy.h"

#include "ProcField.h"

ProcEnemy::ProcEnemy(int idx)
{
	p = iPointZero;
	s = iSizeZero;
	v = iPointZero;

	isActive = false;

	this->idx = (EnemyIndex)idx;
	hp = 0;
	dmg =0;

	up = 0;
	down = 0;
	fall = true;

	sight = 0;
	moveSpeed = 0.f;
	attkRange = 0;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;
	tp = { -1, -1 };
}

ProcEnemy::~ProcEnemy()
{
}

void ProcEnemy::cbAniDead(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	e->isActive = false;
	printf("%s", e->isActive ? "true" : "false");
}

void ProcEnemy::cbAniAttack(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	printf("cb Ani Attack\n");
	e->setState((EnemyBehave)(IdleEnemyL + e->state % 2));
}

void ProcEnemy::initObj(iPoint v)
{
	this->v = v;
}

void ProcEnemy::initObj(iPoint v, EnemyAI ai)
{
	this->v = v;
	this->ai = ai;
}

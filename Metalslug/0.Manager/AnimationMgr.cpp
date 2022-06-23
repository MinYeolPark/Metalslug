#include "AnimationMgr.h"

#include "ProcPlayer.h"
#include "ProcEnemy.h"

void AnimationMgr::cbAniDead(void* parm)
{
	ProcObject* e = (ProcObject*)parm;
	e->isActive = false;
}

void AnimationMgr::cbAniAttack(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	printf("cb Ani Attack\n");
	e->setState((EnemyBehave)(IdleEnemyL + e->state % 2));
}

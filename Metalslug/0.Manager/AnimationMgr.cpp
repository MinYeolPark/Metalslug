#include "AnimationMgr.h"

#include "ProcPlayer.h"
#include "ProcEnemy.h"
#include "ProcNpc.h"

void AnimationMgr::cbAniDead(void* parm)
{
	ProcObject* e = (ProcObject*)parm;
	e->isActive = false;
	printf("cb Ani dead\n");
}

//need fix
void AnimationMgr::cbAniAttack(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	printf("cb Ani Attack\n");
	e->setState(0 + e->state % 2);
}

void AnimationMgr::cbAniEnemyMotion2Idle(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	e->setState(0 + e->state % 2);
}

void AnimationMgr::cbAniMeleeFire(void* parm)
{
	printf("cb ani melee fire\n");
	ProcEnemy* e = (ProcEnemy*)parm;

	//e->setState((EnemyBehave))
}

void AnimationMgr::cbAniNpcRelease(void* parm)
{
	printf("cb ani release");
	ProcNpc* n = (ProcNpc*)parm;

	int r = (rand() % 2 - 0.5) * 2;
	n->v.x = r;
	n->setState((NpcBehave)(WalkNpcL + n->state % 2));
}

void AnimationMgr::cbAniNpcSpawnItem(void* n)
{
	ProcNpc* npc = (ProcNpc*)n;

	npc->complete = true;
	npc->spawnItem();
	npc->setState((NpcBehave)(SaluteNpcL + npc->state % 2));
}

void AnimationMgr::cbAniNpcSalute(void* n)
{
	ProcNpc* npc = (ProcNpc*)n;

	int r = (rand() % 2 - 0.5) * 2;
	npc->v = iPointRotate(iPointMake(r, 0), iPointZero, 0);
	if (npc->v != iPointZero)
	{
		if (npc->v.x > 0)
			npc->setState(EscapeNpcR);
		else if (npc->v.x < 0)
			npc->setState(EscapeNpcL);
	}
}

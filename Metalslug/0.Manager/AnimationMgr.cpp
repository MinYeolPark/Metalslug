#include "AnimationMgr.h"

#include "ProcPlayer.h"
#include "ProcEnemy.h"
#include "ProcNpc.h"
#include "ProcBullets.h"
#include "Truck.h"
#include "BulletMgr.h"

#include "ArabMelee.h"
#include "Mosque.h"
#include "Kessie.h"
void AnimationMgr::cbAniToIdle(void* parm)
{
	ProcPlayer* pp = (ProcPlayer*)parm;

	pp->fireing = false;	
	pp->topState = PlayerIdle;
	pp->botState = PlayerIdle;
}

void AnimationMgr::cbAniToCrouch(void* parm)
{
	ProcPlayer* pp = (ProcPlayer*)parm;

	pp->fireing = false;
	pp->topState = PlayerCrouch;
}

void AnimationMgr::cbAniDead(void* parm)
{
	ProcObject* e = (ProcObject*)parm;
	e->isActive = false;

	if (e->layer == LayerPlayer)
	{
		ProcPlayer* p = (ProcPlayer*)parm;
		p->inviDt = 0.0000001f;
	}

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
	ArabMelee* e = (ArabMelee*)parm;

	e->setState( 6+ e->state % 2);//shuffle
}

void AnimationMgr::cbAniTruck(void* parm)
{
	Truck* t = (Truck*)parm;
	t->_aiDt = 3.f;	
}

void AnimationMgr::cbAniMosque(void* parm)
{
	ProcBullets* b = (ProcBullets*)parm;

	printf("cb Ani mosque\n");
	b->index = BulletMosqueTrace;
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

void AnimationMgr::cbAniBulletDisappear(void* parm)
{
	ProcBullets* b = (ProcBullets*)parm;
	b->isActive = false;
}

void AnimationMgr::cbAniBulletDisappearWithAlpha(void* parm)
{
	ProcBullets* b = (ProcBullets*)parm;
	b->alpha = 0;
}

void AnimationMgr::cbAniKessieBlast(void* parm)
{
	Kessie* k = (Kessie*)parm;
	printf("cb Ani kessieBlast\n");
	for (int i = 0; i < 2; i++)
	{
		if (k->blastState[i] == 1)		//blast Start
			k->blastState[i] = 2;		//blasting
		k->imgLeftBlast[1]->startAnimation(cbAniKessieBlast, k);
		k->imgRightBlast[1]->startAnimation(cbAniKessieBlast, k);
	}
}

void AnimationMgr::cbAniKessieBlastEnd(void* parm)
{
	Kessie* k = (Kessie*)parm;
	printf("cb Ani kessieBlastEnd\n");
	for (int i = 0; i < 2; i++)
	{
		if (k->blastState[i] == 3)		//blast Start
		{
			k->blastState[i] = 1;		//blasting
			k->attkCollider[i]->enable();
			k->imgLeftBlast[3]->startAnimation(cbAniKessieBlastEnd, k);
			k->imgRightBlast[3]->startAnimation(cbAniKessieBlastEnd, k);			
		}
	}
}

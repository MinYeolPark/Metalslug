#include "AnimationMgr.h"

#include "ProcPlayer.h"
#include "ProcEnemy.h"
#include "ProcNpc.h"
#include "ProcBullets.h"
#include "Truck.h"

#include "EffectMgr.h"
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
	e->alphaDt = 0.000001f;
	e->_alphaDt = 0.5f;
	if (e->layer == LayerPlayer)
	{
		ProcPlayer* p = (ProcPlayer*)parm;
		p->inviDt = 0.000001f;
		p->isActive = false;
	}
}

//need fix
void AnimationMgr::cbAniAttack(void* parm)
{
	ProcEnemy* e = (ProcEnemy*)parm;

	printf("cb Ani Attack\n");
	e->state = (0 + e->state % 2);	
}

//void AnimationMgr::cbAniEnemyMotion2Idle(void* parm)
//{
//	ProcEnemy* e = (ProcEnemy*)parm;
//	
//	e->state=(0 + e->state % 2);
//}

void AnimationMgr::cbAniMeleeFire(void* parm)
{
	ArabMelee* e = (ArabMelee*)parm;

	e->state=( 6 + e->state % 2);//shuffle
	//e->tp = e->p;
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
#if 1	
	ProcNpc* n = (ProcNpc*)parm;

	int r = (rand() % 2 - 0.5) * 2;
	n->v.x = r;
	n->state = (NpcBehave)(WalkNpcL + n->state % 2);	
#endif
}

void AnimationMgr::cbAniNpcSpawnItem(void* parm)
{
#if 1
	ProcNpc* n = (ProcNpc*)parm;

	n->complete = true;
	n->spawnItem();
	n->state = (NpcBehave)(SaluteNpcL + n->state % 2);
#endif
}

void AnimationMgr::cbAniNpcSalute(void* parm)
{
#if 1
	ProcNpc* n = (ProcNpc*)parm;

	int r = (rand() % 2 - 0.5) * 2;
	n->v = iPointRotate(iPointMake(r, 0), iPointZero, 0);
	if (n->v != iPointZero)
	{
		if (n->v.x > 0)
			n->state = EscapeNpcR;
		else if (n->v.x < 0)
			n->state = EscapeNpcL;
	}
#endif
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
		{
			k->blastState[i] = 2;		//blasting
			int maxY = *(map->maxY + (int)k->p.x);	
			for (int i = 0; i < 2; i++)
			{
				iPoint pos[2] =
				{
					{k->p.x - 88,(float)maxY+20},
					{k->p.x + 88,(float)maxY+20},
				};
				addProcEffect(k, EffectKessieBlast, pos[i], i);

			}
		}
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
			k->imgLeftBlast[3]->startAnimation(cbAniKessieBlastEnd, k);
			k->imgRightBlast[3]->startAnimation(cbAniKessieBlastEnd, k);
		}
	}
}

void AnimationMgr::cbAniShutterOpen(void* parm)
{
	printf("cbAni Door Open\n");
	Mosque* m = (Mosque*)parm;
	for (int i = 0; i < 3; i++)
	{
		m->curtainState[i] = 1;
		m->shutterState[i] = 1;
	}
}

void AnimationMgr::cbAniCurOpen(void* parm)
{
	
}

void AnimationMgr::cbAniAddOut(void* parm)
{
	Mosque* mt = (Mosque*)parm;

	for (int i = 0; i < 3; i++)
	{

	}
	mt->isActive = false;
}
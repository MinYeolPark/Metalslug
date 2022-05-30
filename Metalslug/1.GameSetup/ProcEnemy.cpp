#include "ProcEnemy.h"
#include "ProcField.h"

#include "EnemyMgr.h"
#include "ArabMelee.h"

#define DEBUG

#if 1
ProcEnemy::ProcEnemy(int idx)
{
	layer = OBJECT_LAYER::Enemy;

	this->idx = (EnemyIndex)idx;
	hp = 0;
	dmg =0;

	up = 0;
	down = 0;
	fall = true;

	state = eIdleL;

	sight = 0;
	attkRange = 0;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;
	tp = iPointZero;
}

ProcEnemy::~ProcEnemy()
{
}

void ProcEnemy::initObj()
{
	
}

void ProcEnemy::updateObj(float dt)
{
	if (getState()==eDeadR || getState()==eDeadL)
		return;

	if (!fall)
	{
		if (tp != iPointZero)
		{
			if (movePoint(p, p, tp, moveSpeed * dt))
			{
				float dp = 0xffffff;
				if (containPoint(p, player->collider()))
				{
					float d = iPointLength(p - player->p);
					if (dp > d)
					{
						dp = d;
					}
				}
			}
		}
	}
	fixedUpdate(dt);
}

void ProcEnemy::fixedUpdate(float dt)
{
	int maxY = *(bg->maxY + (int)p.x);
	if (p.y >= maxY)
	{
		up = 0;
		down = 0;
		fall = false;

		p.y = maxY;
	}
	else
		fall = true;

	if (fall)
	{
		if (p.y < maxY)
		{
			down += jumpDecrease * dt;
			p = (iPointMake(p.x, p.y += down));
		}
	}

	if (tp!=iPointZero)
		tp.y = maxY;	
}

void ProcEnemy::renderObj(float dt, iPoint off)
{

}
void ProcEnemy::releaseObj()
{
}

void ProcEnemy::setState(EnemyBehave eb)
{
	state = eb;
}

void ProcEnemy::updateAi(ProcEnemy* e, float dt)
{
	aiDt += dt;
	if (aiDt > _aiDt)
	{		
		aiDt -= _aiDt;
	}
}

void ProcEnemy::methodDead(void* parm)
{	
	ProcEnemy* pe = (ProcEnemy*)parm;	

	//if(pe->imgCurr->alpha<=0.5f)	
	pe->isActive = false;
}

void ProcEnemy::methodAttack(void* parm)
{
	ProcEnemy* pe = (ProcEnemy*)parm;

	pe->setState((EnemyBehave)(eIdleL + (pe->state % 2)));
	pe->attkRate -= pe->_attkRate;
}

iRect ProcEnemy::collider()
{
	return iRectMake(
		p.x - imgCurr->tex->width / 2 * imgCurr->scale + bg->off.x,
		p.y - imgCurr->tex->height * imgCurr->scale + bg->off.y,
		imgCurr->tex->width * imgCurr->scale, imgCurr->tex->height * imgCurr->scale);
}

iRect ProcEnemy::attkCollider()
{
	return iRectMake(
		p.x - imgCurr->tex->width / 2 * imgCurr->scale + bg->off.x,
		p.y - (imgCurr->tex->height * imgCurr->scale)/2 + bg->off.y,
		(imgCurr->tex->width * imgCurr->scale)/2, (imgCurr->tex->height * imgCurr->scale) / 3);
}
#endif

void aiArabMelee(ProcEnemy* e, float dt)
{	
	printf("Arab ai update\n");
#if 0 
	// idle
	int r = rand() % 100;
	if (r < 10)
	{
		printf("%f/%f\n", e->aiDt, e->_aiDt);
		e->aiDt = -2;// wait duration
		e->setState((EnemyBehave)(eIdleL + (e->state % 2)));
	}
	else
	{
		float d = 50 + rand() % 150;
		e->tp.x = e->p.x + d * (r < 75 ? -1 : 1);

		if(e->p.x-e->tp.x>0)
			e->setState((EnemyBehave)(eMoveL));
		else
			e->setState((EnemyBehave)(eMoveR));
		printf("Shuffling\n");
	}
#endif
	
	//else
	//{
	//	// idle
	//	int r = rand() % 100;
	//	if (r < 50)
	//	{
	//		e->aiDt = -5;// wait duration
	//		e->setState((EnemyBehave)(eIdleLeft + (e->state % 2)));
	//	}
	//	else
	//	{
	//		float d = 50 + rand() % 150;
	//		e->tp.x = e->p.x + d * (r < 75 ? -1 : 1);
	//		e->setState((EnemyBehave)(eMoveLeft + (e->state % 2)));
	//	}
	//
	//}
}
void aiArabCamel(ProcEnemy* e, float dt)
{

}
void aiArabBurseker(ProcEnemy* e, float dt)
{
	iPoint v = player->p - e->p;
	v /= iPointLength(v);
	if (v.x > 0)
	{
		//e->setState((EnemyBehave)(bMoveR));
		e->tp.x = bg->maxW + bg->off.x + 20;
	}
	else
	{
		e->tp.x = bg->off.x + 20;
	}
	
	int d = getDistance(e->p, e->tp);
}


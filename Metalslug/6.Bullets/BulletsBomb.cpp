#include "BulletsBomb.h"

#include "UIMgr.h"
#include "EnemyMgr.h"	
#include "EffectMgr.h"
#include "BulletMgr.h"

#include "ProcNpc.h"
#include "ProcStructure.h"

BulletsBomb::BulletsBomb(int index) : ProcBullets(index)
{
}
BulletsBomb::~BulletsBomb()
{
}

void BulletsBomb::init(ProcObject* parent, int index, float degree, int fpNum)
{
	ProcPlayer* owner = (ProcPlayer*)parent;

	this->isActive = true;
	this->parent = owner;
	this->degree = degree;
	this->p = owner->p;
	this->index = index;
	this->alpha = 1.0f;

	pow = 5.0f;
	down = 0.f;
	up -= pow;	
#if 1
	this->v = owner->dirRight ? iPointMake(1, 0) : iPointMake(-1, 0);
#endif
	this->degree = owner->fireDeg;
	float speedInfo[] = {
		100,
		200,
		200
	};
	this->speed = speedInfo[index];
	float damageInfo[] =
	{
		200,
		200,
		200,
	};
	this->damage = damageInfo[index];	
}

static float delta;
void BulletsBomb::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));		

#if 1
	if (up)
	{
		p = iPointMake(p.x, p.y -= pow);
		up += 9.81 * dt;
		if (up > 0.0f)
			up = 0.0f;
	}

	int maxY = *(map->maxY + (int)p.x);
	if (p.y < maxY)
	{
		down += 9.81 * dt;
		if (v.x > 0)
			p = iPointMake(p.x + 2, p.y += down);
		else// if(!b->parent->isRight)
			p = iPointMake(p.x - 2, p.y += down);		
	}
	else
	{
		if (pow < 1.5f)
		{
			addProcEffect(this, EffectBomb, p);
			audioPlay(snd_eff_bomb);
			isActive = false;
		}
		pow *= 0.7f;
		up -= pow;
		down = 0.f;
	}
#endif

	ProcObject* dst = NULL;
	for (int i = 0; i < bulletNum; i++)
	{
		ProcBullets* b = bullets[i];
		float dMin = 0xfffff;
		float d = iPointLength(b->p - p);
		if (dMin > d)
		{
			dMin = d;
			if (b->rect)
			{
				dst = b;
				for (int j = 0; j < dst->rectNum; j++)
				{
					if (containPoint(p + map->off, dst->getRect(j)))
					{
						isActive = false;
						dst->getDamage(damage);
						iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
						addProcEffect(this, index, bp);		//bulletIndex=effectIndex
					}
				}
			}
		}
	}
	for (int i = 0; i < enemyNum; i++)
	{
		ProcEnemy* e = enemies[i];
		float dMin = 0xfffff;
		float d = iPointLength(e->p - p);
		if (dMin > d)
		{
			dMin = d;
			if (e->rect)
			{
				dst = e;
				for (int j = 0; j < dst->rectNum; j++)
				{

					if (containPoint(p + map->off, dst->getRect(j)))
					{
						isActive = false;
						dst->getDamage(damage);
						iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
						addProcEffect(this, index, bp);		//bulletIndex=effectIndex
					}
				}
			}
		}
	}
	for (int i = 0; i < structNum; i++)
	{
		ProcStructure* s = structures[i];
		float dMin = 0xfffff;
		float d = iPointLength(s->p - p);
		if (dMin > d)
		{
			dMin = d;
			if (s->rect)
			{
				dst = s;
				for (int j = 0; j < dst->rectNum; j++)
				{
					if (containPoint(p + map->off, dst->getRect(j)))
					{
						isActive = false;
						dst->getDamage(damage);
						iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
						addProcEffect(this, index, bp);		//bulletIndex=effectIndex
					}
				}
			}
		}
	}
	for (int i = 0; i < npcNum; i++)
	{
		ProcNpc* s = npcs[i];
		float dMin = 0xfffff;
		float d = iPointLength(s->p - p);
		if (dMin > d)
		{
			dMin = d;
			if (s->rect)
			{
				dst = s;
				for (int j = 0; j < dst->rectNum; j++)
				{
					if (s->state == IdleNpcL + s->state % 2)
					{
						if (containPoint(p + map->off, dst->getRect(j)))
						{
							isActive = false;
							dst->getDamage(damage);
							iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
							addProcEffect(this, index, bp);		//bulletIndex=effectIndex
						}
					}
				}
			}
		}
	}
}

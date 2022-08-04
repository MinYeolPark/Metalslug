#include "BulletsPlayer.h"

#include "EnemyMgr.h"	
#include "EffectMgr.h"
#include "BulletMgr.h"

#include "ProcNpc.h"
#include "ProcStructure.h"
#include "ProcNpc.h"
BulletsPlayer::BulletsPlayer(int index) : ProcBullets(index)
{

}
BulletsPlayer::~BulletsPlayer()
{
}

void BulletsPlayer::init(ProcObject* parent, int index, float degree, int fpNum)
{
	ProcPlayer* owner = (ProcPlayer*)parent;

	this->isActive = true;
	this->parent = owner;
	this->degree = degree;
	this->p = owner->fp;
	this->index = index;
	this->alpha = 1.0f;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);
	this->degree = owner->fireDeg;
	float speedInfo[] = {
		350,
		350,
		200,
	};
	this->speed = speedInfo[index];
	float damageInfo[] =
	{
		100,
		120,
		200,
	};
	this->damage = damageInfo[index];	
}

void BulletsPlayer::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));	
	
	if (containPoint(p, iRectMake(p.x, (float) * (map->maxY + (int)p.x),
		15, 15)))
	{
		isActive = false;	
		addProcEffect(this, index, p);		//bulletIndex=effectIndex
	}

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
					if (s->state==IdleNpcL + s->state % 2)
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
	p += v * speed * dt;
}

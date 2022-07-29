#include "BulletsPlayer.h"

#include "EnemyMgr.h"	
#include "EffectMgr.h"
#include "BulletMgr.h"

#include "ProcNpc.h"
#include "ProcStructure.h"
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
		300,
		300,
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
						setRGBA(1, 0, 0, 1);
						drawRect(dst->getRect(j));
						setRGBA(1, 1, 1, 1);

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
						setRGBA(1, 0, 0, 1);
						drawRect(dst->getRect(j));
						setRGBA(1, 1, 1, 1);

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
	//if (dst)
	//{
	//	iRect r;
	//	for (int i = 0; i < dst->rectNum; i++)
	//	{
	//		r = dst->getRect(i);
	//		if (containPoint(p + map->off, r))
	//		{
	//			setRGBA(1, 0, 0, 1);
	//			drawRect(r);
	//			setRGBA(1, 1, 1, 1);

	//			isActive = false;
	//			dst->getDamage(damage);
	//			iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
	//			addProcEffect(index, bp);		//bulletIndex=effectIndex
	//		}
	//	}
	//}
#if 0
	for (int i = 0; i < objects->count; i++)
	{
		Collider* c = (Collider*)objects->objectAtIndex(i);
		if (c->parent->layer != LayerPlayer)
		{
			if (containPoint(p, c->getCollider()))
			{
				float d = iPointLength(p - c->p);
				if (dNear > d)
				{
					if (c->isActive &&
						c->damageable)
					{
						dNear = d;
						cNear = c;
					}
				}
			}
			if (cNear)
			{
				if (cNear->isActive)
				{
					isActive = false;
					cNear->parent->getDamage(damage, cNear);
					iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
					addProcEffect(index, bp);		//bulletIndex=effectIndex
				}
			}
		}
		cNear = NULL;
	}
#endif

	p += v * speed * dt;
}

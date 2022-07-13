#include "BulletsEnemy.h"

#include "EffectMgr.h"
BulletsEnemy::BulletsEnemy(int index) : ProcBullets(index)
{
}

BulletsEnemy::~BulletsEnemy()
{
}

void BulletsEnemy::init(ProcObject* parent, int index, float degree, int fpNum)
{
	ProcEnemy* owner = (ProcEnemy*)parent;

	this->isActive = true;
	this->parent = owner;
	this->degree = degree;
	this->p = owner->firePoint;
	this->index = index;
	this->alpha = 1.0f;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);

	iSize colSize;
	if (index == BulletMelee)
	{
		this->pow = 5.0f;
		this->up -= pow;
		this->speed = 100.f;
		this->damage = 100.f;
		colSize = { 40, 40 };
	}
#if 1
	colNum = 1;
	for (int i = 0; i < colNum; i++)
	{
		colliders[i]->init(this, iSizeMake(40, 40));
		colliders[i]->enable();
		objects->addObject(colliders[i]);
	}
#endif
}

void BulletsEnemy::update(float dt)
{		

	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	if (index == BulletMeleeEnd)
	{
		alpha -= dt;
		if (alpha < 0.0f)
		{
			alpha = 0.0f;
			isActive = false;
			return;
		}
	}

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
		if (parent->isRight)
			p = iPointMake(p.x + 2, p.y += down);
		else// if(!b->parent->isRight)
			p = iPointMake(p.x - 2, p.y += down);
	}
	else
	{
		p.y = maxY;
		index = BulletMeleeEnd;
		for (int i = 0; i < colNum; i++)
			colliders[i]->isActive = false;
		up = 0.0f;
		down = 0.0f;
		return;
	}


	fixedUpdate(dt);
}

void BulletsEnemy::fixedUpdate(float dt)
{
	for (int i = 0; i < player->colNum; i++)
	{
		if (containPoint(p, player->colliders[i]->getCollider()))
		{
			if (!player->isDead)
			{
				isActive = false;
				player->getDamage(damage, player->colliders[i]);
				iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
				addProcEffect(index, bp);		//bulletIndex=effectIndex
			}
		}
	}

	for (int i = 0; i < colNum; i++)
		colliders[i]->update(p, degree, dt);
}

void BulletsEnemy::getDamage(float damage, Collider* c)
{
}

#include "BulletsEnemy.h"

#include "EffectMgr.h"
#include "ArabMelee.h"
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
		
	if (index == BulletMelee)
	{
		this->pow = 5.0f;
		this->up -= pow;
		this->down = 0.f;
		this->speed = 100.f;
		this->damage = 100.f;	
	}	
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
		ProcEnemy* e = (ProcEnemy*)parent;
		if (e->state==PreAttackMeleeR)
			p = iPointMake(p.x + 2, p.y += down);
		else// if(!b->parent->isRight)
			p = iPointMake(p.x - 2, p.y += down);
	}
	else
	{
		p.y = maxY;
		index = BulletMeleeEnd;
		//collider disable
		up = 0.0f;
		down = 0.0f;
		return;
	}


	fixedUpdate(dt);
}

void BulletsEnemy::fixedUpdate(float dt)
{		
	if (containPoint(p + map->off, player->getRect()))
	{
		isActive = false;
		player->getDamage(100);
		iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
		addProcEffect(this, index, bp);		//bulletIndex=effectIndex
	}
}

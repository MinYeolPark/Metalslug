#include "ProcEnemy.h"

#include "ProcPlayer.h"
#include "EnemyMgr.h"
#include "ProcField.h"

ProcEnemy::ProcEnemy(int index) : ProcObject()
{
	layer = LayerEnemy;

	isActive = false;
	this->index = index;
	////////////////////////
	state = 0;
	ai = NULL;

	hp = 0;
	dmg = 0;
	sight = 0;
	moveSpeed = 0.f;
	attkRange = 0.f;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;
	isDead = false;
	tp = { -1, -1 };
}

ProcEnemy::~ProcEnemy()
{
}

void ProcEnemy::getDamage(float damage)
{

}

void ProcEnemy::setState(int newState)
{
	state = newState;
}

void ProcEnemy::init(int index, iPoint p, iPoint v)
{
	this->index = index;
	this->p = p;
	this->v = v;
	if (v.x > 0)
		setState(0);
	else if(v.x<0)
		setState(1);

	objects->addObject(this);
}

void ProcEnemy::fixedUpdate(float dt)
{
	collider->setPosition(p);

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

	if (tp != iPointZero)
		tp.y = maxY;
}

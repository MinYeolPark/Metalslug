#include "ProcEnemy.h"

#include "ProcPlayer.h"
#include "EnemyMgr.h"
#include "ProcField.h"

ProcEnemy::ProcEnemy(int index) : ProcObject()
{
	layer = LayerEnemy;
	p = iPointZero;
	tp = p;
	v = iPointZero;
	s = iSizeZero;

	degree = 0;

	state = 0;

	up = 0.0f;
	down = 0.0f;
	fall = false;
	ai = NULL;

	fpNum = 0;
	fp = { p.x, p.y - 20 };
	////////////////////////
	dmg = 0;
	sight = 0;
	moveSpeed = 0.f;
	attkRange = 0.f;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;

	isDead = false;
	isAppear = false;
}

ProcEnemy::~ProcEnemy()
{
}

void ProcEnemy::setState(int newState)
{
	state = newState;
}

void ProcEnemy::init(int index, iPoint p, iPoint v)
{
	this->isActive = true;
	this->index = index;
	this->p = p;
	this->tp = p;
	this->v = v;	
	if (v.x > 0)
		setState(0);
	else if (v.x < 0)
		setState(1);
	else
		setState(0);
		
	for (int i = 0; i < colNum; i++)
	{
		//init
		colliders[i]->enable();
		objects->addObject(colliders[i]);
	}	
}

void ProcEnemy::fixedUpdate(float dt)
{
	int maxY = *(map->maxY + (int)p.x);
	for (int i = 0; i < objects->count; i++)
	{
		Collider* c = (Collider*)objects->objectAtIndex(i);
		//if (c->isTrigger)
		//{
		//	if (p.y > c->p.y)
		//		return;
		//}
		if (c->isTrigger == true
			&& c->parent != this)
			maxY = c->p.y - c->s.height;		
	}	
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

	for (int i = 0; i < colNum; i++)
		colliders[i]->update(p, degree, dt);
}

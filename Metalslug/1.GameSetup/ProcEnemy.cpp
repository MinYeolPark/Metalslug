#include "ProcEnemy.h"

#include "EnemyMgr.h"
#include "ProcPlayer.h"
#include "ProcField.h"

ProcEnemy::ProcEnemy(int index) : ProcObject()
{
	layer = LayerEnemy;

	p = iPointZero;
	tp = p;
	v = iPointZero;
	s = iSizeZero;

	degree = 0;
	alpha = 1.0f;

	index = 0;
	isDead = false;
	isActive = false;

	hp = 0; _hp = 0;
	score = 0;
	//////////////////////// ProcObject
	state = 0;

	up = 0.0f;
	down = 0.0f;
	fall = false;

	firePoint = p;
	dmg = 0;
	sight = 0;
	moveSpeed = 0.f;
	attkRange = 0.f;
	attkRate = 0.f, _attkRate = 0.f;
	aiDt = 0.f, _aiDt = 0.f;
	isAppear = false;	
}

ProcEnemy::~ProcEnemy()
{
}

void ProcEnemy::getDamage(float damage)
{
}

void ProcEnemy::fixedUpdate(float dt)
{
	int maxY = *(map->maxY + (int)p.x);
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
}

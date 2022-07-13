#include "BulletsPlayer.h"

#include "EffectMgr.h"
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
		110,
	};
	this->speed = speedInfo[index];
	float damageInfo[] =
	{
		100,
		120,
	};
	this->damage = damageInfo[index];
#if 1
	colNum = 1;
	iSize bs[] = {
		{20,10},		//Handgun,
		{40,10},		//Machinegun
	};
	for (int i = 0; i < colNum; i++)
		colliders[i]->init(parent, bs[index]);
#endif		
}

void BulletsPlayer::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));	

	Collider* cNear = NULL;
	float dNear = 0xffffff;
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
					if (c->isActive)
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

	p += v * speed * dt;
}

#include "BulletsMosque.h"

#include "EffectMgr.h"
BulletsMosque::BulletsMosque(int index): ProcBullets(index)
{
	rp = p;
	_hp = 500;
	hp = _hp;
	wave = 0.f;
	d = 0.f;
	_d = 0.f;
	imgCurr = imgs[index];
}

BulletsMosque::~BulletsMosque()
{
}


void BulletsMosque::init(ProcObject* parent, int index, float degree, int fpNum)
{
	Mosque* owner = (Mosque*)parent;

	this->isActive = true;
	this->parent = owner;
	this->degree = degree;
	this->p = owner->firePoint[fpNum];
	this->index = index;
	this->alpha = 1.0f;
	this->v = iPointRotate(iPointMake(1, 0), iPointZero, degree);	

	iSize colSize;
	if (index == BulletMosque)
	{
		this->speed = 10.f;
		this->damage = 0.f;		
		colSize = { 0,0 };
	}
	else if (index == BulletMosqueTrace)
	{
		this->speed = 100.f;
		this->damage = 100.0f;
		colSize = { 40,40 };
	}
#if 0
	colNum = 1;
	for (int i = 0; i < colNum; i++)
	{
		colliders[i]->init(this, iSizeMake(40, 40));
		colliders[i]->enable();
		objects->addObject(colliders[i]);
	}
#endif

	rp = p;
	wave = 15.f;
	d = 0.f;
	_d = 0.f;
}

void BulletsMosque::update(float dt)
{
	if (hp > 0)
	{
		isActive = containPoint(p,
			iRectMake(-map->off.x - 20, -map->off.y - 20,
				devSize.width + 40, devSize.height + 40));
	}

	if (index == BulletMosque)
	{		
		degree = 0;
		if (imgCurr->frame == 29)
		{
			imgs[index]->startAnimation();
			index = BulletMosqueTrace;
		}
		p.x += speed * dt;
	}
	else if (index == BulletMosqueTrace)
	{
		d += iPointLength(v);
		if (d > _d)
			d -= _d;
		float r = d / _d;		
		float angle = iPointAngle(iPointMake(1, 0), iPointZero, v);			
		iPoint w = iPointRotate(iPointMake((wave * _sin(360 * r)), 0), iPointZero, angle + 90);		
		degree = 45 * _sin(360 * r);	
		rp = p + w;

		p += v * speed * dt;
	}

	fixedUpdate(dt);
}


void BulletsMosque::fixedUpdate(float dt)
{
	
}

void BulletsMosque::getDamage(float damage, Collider* c)
{
	printf("damage= %f, hp = %d\n", damage, hp);
	hp -= damage;
	if (hp <= 0)
	{
		isActive = false;
		addProcEffect(index, p);
	}
}

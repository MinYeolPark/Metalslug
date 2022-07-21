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
	for (int i = 0; i < rectNum; i++)
		delete rect[i];
	delete rect;
}


void BulletsMosque::init(ProcObject* parent, int index, float degree, int fpNum)
{
	Mosque* owner = (Mosque*)parent;

	this->isActive = true;
	this->parent = owner;
	this->degree = degree;
	this->p = owner->firePoint[fpNum];
	this->index = index;
	this->parentIndex = fpNum;
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

	rp = p;
	wave = 15.f;
	d = 0.f;
	_d = 0.f;
	_hp = 300;
	hp = _hp;

	rectNum = 1;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();
	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(20, 20);
		r->origin = p;
	}
}

void BulletsMosque::update(float dt)
{
	Mosque* owner = (Mosque*)parent;
	if (owner->towerState[parentIndex] == MosqueDead)
		getDamage(_hp);
	if (isDead)
	{
		for (int i = 0; i < rectNum; i++)
			rect[i]->size = iSizeZero;
		return;
	}

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
	//ColliderUpdate
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			p.x + map->off.x - rect[i]->size.width / 2,
			p.y + map->off.y - rect[i]->size.height);
	}
}

void BulletsMosque::getDamage(float damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		isActive = false;
		addProcEffect(this, index, p);
	}
}

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
	this->speed = 20.f;
	this->damage = 0.f;

	tp = parent->tp;
	rp = p;
	wave = 15.f;
	d = 0.f;
	_d = 100.f;
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
			speed = 50.f;
			damage = 100.f;
			for (int i = 0; i < rectNum; i++)
			{
				iRect* r = rect[i];
				r->size = iSizeMake(30, 30);
				r->origin = p;
			}
			p = rp;
		}
		rp.x += speed * dt;
	}
	else if (index == BulletMosqueTrace)
	{
		iPoint md = v * speed * dt;
		p += md;
		d += iPointLength(md);
		if (d > _d)
			d -= _d;
		float rate = d / _d;
		float add = _cos(rate * 360) * 30;
		iPoint h = iPointMake(v.y, -v.x);
		rp = p + h * (_sin(rate * 360) * wave);
		float angle = iPointAngle(iPointMake(1, 0), iPointZero, v);
		angle += add;
		degree = 360-angle;
	}

	fixedUpdate(dt);
}


void BulletsMosque::fixedUpdate(float dt)
{	
	if (containPoint(rp + map ->off, player->getRect()))
	{
		isActive = false;
		player->getDamage(100);
		iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
		addProcEffect(this, EffectBomb, bp);
	}

	//Check Ground
	if (containPoint(p, iRectMake(p.x, (float)*(map->maxY + (int)p.x),
		15, 15)))
	{
		isActive = false;
		addProcEffect(this, index, p);
	}


	//ColliderUpdate
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			rp.x + map->off.x - rect[i]->size.width / 2,
			rp.y + map->off.y - rect[i]->size.height);
	}
}

bool BulletsMosque::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, alpha);
	imgCurr = imgs[index];
	imgCurr->degree = degree;
	imgCurr->paint(dt, rp + off);	
	setRGBA(1, 1, 1, 1);

#ifdef DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect());
#endif // _DEBUG

	return !isActive;
}

void BulletsMosque::getDamage(float damage)
{
	hp -= damage;
	player->addScore(score);
	if (hp <= 0)
	{
		isActive = false;
		addProcEffect(this, index, p);
	}
}

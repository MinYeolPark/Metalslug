#include "Collider.h"

#include "InputMgr.h"
#define colliderMax 50
Collider* _colliders;
Collider** colliders;
int colNum;
void loadProcColliders()
{
	_colliders = new Collider[colliderMax];
	colliders = new Collider * [colliderMax];
	colNum = 0;
}

void freeProcColliders()
{
	delete _colliders;
	delete colliders;
}

void drawProcColliders(float dt, iPoint off)
{
	for (int i = 0; i < colNum; i++)
	{
		Collider* c = colliders[i];
		c->update(dt);
		if (c->draw(dt, off))
		{
			colNum--;
			printf("colNum=%d\n",colNum);
			colliders[i] = colliders[colNum];
			i--;
		}
	}
}

void addColliders(ProcObject* parent, iSize s)
{
	for (int i = 0; i < colliderMax; i++)
	{
		Collider* c = &_colliders[i];
		if (!c->isActive)
		{			
			c->init(parent, s);
			colliders[colNum] = c;
			colNum++;
			return;
		}
	}
}

void freeArrCollider(void* data)
{
	Collider* c = (Collider*)data;

	c->disable();
	delete c;
}
iArray* procColliders = new iArray(freeArrCollider);
Collider::Collider()
{
	p = iPointZero;
	s = iSizeZero;
	degree = 0;
	alpha = 0.f;

	parent = NULL;

	colID = 0;
	isActive = false;
	isTrigger = false;
	damageable = false;
}
Collider::~Collider()
{

}
void Collider::init(ProcObject* parent, iSize s)
{
	isActive = true;
	this->parent = parent;
	this->s = s;
	colID = colNum;
}

void Collider::update(float dt)
{
	isActive = !parent->isDead;
	printf("#%d Parent is dead ? =", colID);
	printf(parent->isDead ? "true\n" : "false\n");
	p = parent->p;
	degree = parent->degree;
}

bool Collider::draw(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, alpha);
#ifdef _DEBUG
	alpha = 1;
#endif // DEBUG

	drawRect(getCollider());
	setRGBA(1, 1, 1, 1);
	return !isActive;
}

void Collider::setPosition(iPoint p)
{
	this->p = p;
}
void Collider::setSize(iSize s)
{
	this->s = s;
}

iRect Collider::getCollider()
{
	if (isActive)
	{
		return iRectMake(p.x + map->off.x - s.width / 2,
			p.y + map->off.y - s.height,
			s.width, s.height);
	}
	return iRectMake(0, 0, 0, 0);
}

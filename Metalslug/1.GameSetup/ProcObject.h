#pragma once

#include "iStd.h"
enum ObjLayer
{
	LayerDefault = 0,
	LayerPlayer,
	LayerEnemy,	
	ObjLayerMax = 16
};
//Components
struct Collider;
class ProcObject
{
public:
	ProcObject();
	virtual ~ProcObject();
public:
	ObjLayer layer;

	iPoint p;					//position
	iPoint v;					//direction
	iSize s;					//size

	bool isActive;
	int index;
	int hp, _hp;
	int score;
	Collider* collider;
public:
	virtual void getDamage(float damage) = 0;
	virtual int getScore();
public:
	virtual void init(iPoint p);
	virtual void init(iPoint p, iPoint v);
	virtual void init(int index, iPoint p, iPoint v);
	virtual void update(float dt) = 0;				//With control physics
	virtual bool draw(float dt, iPoint off) = 0;	
};
static void cb(void* data);
extern iArray* objects;
extern iArray* objectsRemove;
#include "ProcField.h"
struct Collider
{
	Collider()
	{
		p = iPointZero;
		s = iSizeZero;
		isActive = false;
	}
	~Collider()
	{

	}

	iPoint p;
	iSize s;
	ProcObject* parent;
	bool isActive;

	void init(ProcObject* parent, iSize s)
	{
		this->parent = parent;
		parent->collider->isActive = true;
		this->p = { parent->p.x - s.width/2, parent->p.y - s.height };
		this->s = s;
	}
	void setPosition(iPoint p)
	{
		this->p = p;
	}
	void setSize(iSize s)
	{
		this->s = s;
	}
	iRect getCollider()
	{
		if (isActive)
		{
			return iRectMake(parent->p.x+bg->off.x - s.width/2, parent->p.y+bg->off.y-s.height,
				s.width, s.height);
		}
		return iRectMake(0,0,0,0);
	}
	void enable() { isActive = true; }
	void disable() { isActive = false; }
};

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
	virtual void getDamage(float damage, iPoint hitPoint);
	virtual int getScore();
	virtual void dead() = 0;
public:
	//virtual iRect collider() = 0;
	virtual void init(iPoint p);
	virtual void init(iPoint p, iPoint v);
	virtual void init(int index, iPoint p, iPoint v);
	virtual void update(float dt) = 0;				//With control physics
	virtual bool draw(float dt, iPoint off) = 0;	
};
static void cb(void* data);
extern iArray* objects;

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

	bool isActive;

	void init(ProcObject* parent)
	{
		p = { parent->p.x - 25, parent->p.y - 50 };
		s = { 50,50 };
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
		//if (isActive)
		//{
		//	return iRectMake(p.x, p.y, s.width, s.height);
		//}
		return iRectMake(p.x, p.y, s.width, s.height);
	}
	void enable() { isActive = true; }
	void disable() { isActive = false; }
};

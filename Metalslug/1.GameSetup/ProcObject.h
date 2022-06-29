#pragma once

#include "iStd.h"
enum ObjLayer
{
	LayerDefault = 0,
	LayerPlayer,
	LayerEnemy,	
	LayerMosque,
	LayerKessie,

	ObjLayerMax = 16
};
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

	int colNum;
	Collider* colliders[3];
public:
	virtual void getDamage(float damage, Collider* c) = 0;
	virtual int getScore();
public:
	virtual void init(iPoint p);
	virtual void init(iPoint p, iPoint v);
	virtual void init(int index, iPoint p, iPoint v);
	virtual void update(float dt) = 0;				//With control physics
	virtual bool draw(float dt, iPoint off) = 0;	
};
extern iArray* objects;
#include "ProcField.h"
struct Collider
{
	Collider()
	{
		p = iPointZero;
		s = iSizeZero;
		parent = NULL;
		isActive = false;
		isTrigger = false;
	}
	~Collider()
	{

	}

	iPoint p;
	iSize s;
	ProcObject* parent;
	bool isActive;
	bool isTrigger;
	void init(ProcObject* parent, iSize s)
	{
		this->p = { parent->p.x - s.width/2, parent->p.y - s.height };
		this->s = s;
		this->parent = parent;
		if (parent->colNum > 3)
		{
			printf("colNum error\n");
			return;
		}		
		for (int i = 0; i < parent->colNum; i++)
			parent->colliders[i]->enable();
		isTrigger = false;
	}
	void update(ProcObject* parent)
	{
		this->p = parent->p;

		if (parent->colNum == 3)
		{
			for (int i = 0; i < 3; i++)
			{
				if (parent->colliders[i]->isActive == false)
					delete parent->colliders[i];
			}
		}
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
			return iRectMake(p.x + map->off.x - s.width / 2, 
				p.y + map->off.y - s.height,
				s.width, s.height);
		}
		return iRectMake(0,0,0,0);
	}
	void enable() { isActive = true; }
	void disable() { isActive = false; }
};

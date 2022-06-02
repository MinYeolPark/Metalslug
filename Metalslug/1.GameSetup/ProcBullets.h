#pragma once

#include "ProcObject.h"

enum BulletIndex
{
	BulletHandGun = 0,
	BulletHeavyMachinegun,

	MoskMissile,
	BulletIndexMax,
};

class ProcBullets;
typedef void(*BulletPattern)(ProcBullets* b, float dt);
class ProcBullets: public ProcObject
{
public:
	ProcBullets(int idx);
	virtual ~ProcBullets();

public:
	iImage** imgs;
	iImage* imgCurr;

	ProcObject* parent;
	int bulletIdx;

	int dir;
	iPoint v;

	float speed;
	float dmg;

	BulletPattern pattern;
public:
	virtual void initObj();
	virtual void updateObj(float dt);
	virtual void drawObj(float dt, iPoint off);
	virtual void freeObj();

	//Components
public:
	virtual iRect collider()
	{
		return iRectMake(p.x, p.y,
			imgCurr->tex->width, imgCurr->tex->height);
	}
};
//Patterns
BulletPattern methodDefault(ProcBullets* b, float dt);
BulletPattern methodMosk(ProcBullets* b, float dt);

#if 0
struct Animal
{
	iImage** imgs, *imgCurr;
	int imgsNum;
	int be;

	iPoint position;
	float rate, _rate;
	float ap;

	Animal()
	{
		imgsNum = 0;
		//
	}

	virtual ~Animal()
	{
		for (int i = 0; i < imgsNum; i++)
			delete imgs[i];
		delete imgs;
	}

	void paint(float dt, iPoint off)
	{
		imgCurr->paint(dt, off);
	}
};

struct Enemy : Animal
{
	Enemy() : Animal()
	{
		// add new
	}

	virtual ~Enemy()
	{
		// only delete me
	}

	void paint(float dt, iPoint off)
	{
		updateAI(dt, off);
	
		Animal::paint(dt, off);
	}

	virtual void updateAI(float dt, iPoint off) = 0;
};
#define DECLARE_METHODAI_START(className, classParent, df, off) struct className : classParent { virtual void updateAI(float dt, iPoint off)
#define DECLARE_METHODAI_END };

DECLARE_METHODAI_START(EnemyMos, Enemy, dt, off)
{

}DECLARE_METHODAI_END

DECLARE_METHODAI_START(EnemyAAA, Enemy, dt, off)
{
}DECLARE_METHODAI_END

DECLARE_METHODAI_START(EnemyBBB, Enemy, dt, off)
{
}DECLARE_METHODAI_END



struct Player : Animal
{
	Player() : Animal()
	{
		// add new
	}

	virtual ~Player()
	{
		// only delete me
	}

	void paint(float dt, iPoint off)
	{
		ctrl(dt);

		Animal::paint(dt, off);
	}

	void ctrl(float dt)
	{

	}
};



struct Npc : Animal
{
	Npc() : Animal()
	{
		// add new
	}

	virtual ~Npc()
	{
		// only delete me
	}

	void paint(float dt, iPoint off)
	{
		Animal::paint(dt, off);
	}
};
#endif
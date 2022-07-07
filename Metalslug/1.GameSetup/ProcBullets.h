#pragma once

#include "iStd.h"

#include "Mosque.h"
#include "ProcObject.h"
#include "ProcEnemy.h"
#include "ProcPlayer.h"

class ProcBullets;
typedef void(*BulletPattern)(ProcBullets* b, float dt);
enum BulletIndex
{
	BulletHandGun = 0,
	BulletHeavyMachinegun,
	BulletBomb,

	BulletMelee,
	BulletMosque,
	BulletMosqueTrace,

	//No Effect
	BulletMeleeEnd,
	BulletIndexMax,
};

typedef void (*BulletPattern)(ProcBullets* b, float dt);
class ProcBullets
{
public:
	ProcBullets(int idx);
	virtual ~ProcBullets();

public:
	int layer;
	iImage** imgs;
	iImage* imgCurr;

	ProcObject* parent;
	int index;

	iPoint p;
	iPoint v;

	float degree;
	bool isActive;

	float alpha;

	float pow;
	float up;
	float down;

	float speed;
	float damage;

	BulletPattern pattern;
public:
	virtual void init(ProcObject* parent, int index, float degree);
	virtual void init(ProcPlayer* parent, int idx, float degree);
	virtual void init(ProcEnemy* parent, int idx, float degree);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();

	//Components
public:
	Collider* collider;
};


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
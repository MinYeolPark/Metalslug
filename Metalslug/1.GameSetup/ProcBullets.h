#pragma once

#include "iStd.h"

#include "Mosque.h"
#include "ProcObject.h"
#include "ProcEnemy.h"
#include "ProcPlayer.h"

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

class ProcBullets:
	public ProcObject
{
public:
	ProcBullets(int idx);
	virtual ~ProcBullets();

public:
	iImage** imgs;
	iImage* imgCurr;

	ProcObject* parent;
	float degree;

	float pow;
	float up;
	float down;

	float speed;
	float damage;

public:
	virtual void init(ProcObject* parent, int index, float degree, int fpNum) = 0;	
	virtual void update(float dt) = 0;
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();

public:
	virtual void getDamage(float damage);
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
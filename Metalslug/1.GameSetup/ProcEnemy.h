#pragma once
#include "ProcObject.h"

enum EnemyIndex
{
	IdxMelee = 0,
	IdxMeleeKessie,
	IdxMosque,
	IdxBurserker,
	IdxArCamel,
	IdxTruck,
	IdxKessie,
	IdxAbul,

	EnemyIndexMax,
};
class ProcEnemy: 
	public ProcObject
{
public:
	ProcEnemy(int index);
	virtual ~ProcEnemy();
public:
	int state;

	float up;
	float down;
	bool fall;

	iPoint firePoint;
	//stats
	int dmg;
	float sight;
	float moveSpeed;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;

	bool isAppear;
public:	
	virtual void getDamage(float damage);
public:
	virtual void init(iPoint p) = 0;
	virtual void fixedUpdate(float dt);
	virtual void free() = 0;		//freeImage
};


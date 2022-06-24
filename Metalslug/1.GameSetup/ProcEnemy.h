#pragma once
#include "ProcObject.h"

enum EnemyIndex
{
	IdxArMelee = 0,
	IdxArBurserker,

	IdxArCamel,
	IdxKessie,
	IdxMasknell,
	IdxMosque,
	EnemyIndexMax,
};

class ProcEnemy;
typedef void(*EnemyAI)(ProcEnemy* e, float dt);
class ProcEnemy: 
	public ProcObject
{
public:
	ProcEnemy(int index);
	virtual ~ProcEnemy();
public:
	int state;
	EnemyAI ai;
	//stats
	int hp;
	int dmg;
	float sight;
	float moveSpeed;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;

	float up;
	float down;
	bool fall;
	bool isDead;
	iPoint tp;
public:
	virtual bool dead() = 0;

	virtual void getDamage(float damage) = 0;
	virtual void setState(int newState) = 0;
public:
	virtual void init(int index, iPoint p, iPoint v);
	virtual void fixedUpdate(float dt);
	virtual void free() = 0;		//freeImage
};


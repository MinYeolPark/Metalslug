#pragma once
#include "ProcObject.h"

enum EnemyIndex
{
	IdxArMelee = 0,
	IdxArBurserker,

	IdxMasknell,
	IdxMosque,
	IdxKessie,
	EnemyIndexMax,
};

enum EnemyBehave
{
	IdleEnemyL = 0,
	IdleEnemyR,

	WalkEnemyL,
	WalkEnemyR,

	DeadEnemyL,
	DeadEnemyR,

	ShuffleEnemyL,
	ShuffleEnemyR,

	AttackEnemyL,
	AttackEnemyR,

	PreAttackEnemyL,
	PreAttackEnemyR,

	FireEnemyL,
	FireEnemyR,

	EnemyBehaveMax,
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
	EnemyBehave state;
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

	iPoint tp;
public:
	virtual bool dead() = 0;

	virtual void getDamage(float damage);
	virtual void setState(EnemyBehave newState) { state=newState; }
	virtual EnemyBehave getState() { return state; }
public:
	virtual void init(int index, iPoint p, iPoint v);
	virtual void fixedUpdate(float dt);
	virtual void free() = 0;		//freeImage
};


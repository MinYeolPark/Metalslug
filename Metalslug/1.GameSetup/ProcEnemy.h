#pragma once
#include "ProcObject.h"

enum EnemyIndex
{
	ArMelee = 0,
	ArBurserker,

	IdxMasknell,
	IdxMosque,
	IdxKessie,
	EnemyIndexMax,
};

enum EnemyBehave
{
	IdleEnemyL,
	IdleEnemyR,

	WalkEnemyL,
	WalkEnemyR,

	DeadEnemyL,
	DeadEnemyR,

	ShuffleEnemyL,
	ShuffleEnemyR,

	AttackEnemyL,
	AttackEnemyR,

	EnemyBehaveMax,
};

class ProcEnemy;
typedef void(*EnemyAI)(ProcEnemy* e, float dt);
class ProcEnemy
{
public:
	ProcEnemy(int idx);
	virtual ~ProcEnemy();

public:
	iPoint p;
	iPoint v;
	iSize s;

	bool isActive;

	EnemyIndex idx;
	EnemyBehave state;
	EnemyAI ai;
	float up;
	float down;
	bool fall;

	//stats
	int hp;
	int dmg;
	float sight;
	float moveSpeed;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;
	iPoint tp;

public:
	virtual iRect collider() = 0;
public:
	virtual void initObj() = 0;
	virtual void initObj(iPoint v);
	virtual void initObj(iPoint v, EnemyAI ai);
	virtual void updateObj(float dt) = 0;
	virtual void fixedUpdate(float dt) = 0;
	virtual void drawObj(float dt, iPoint off) = 0;
	virtual void freeObj() = 0;
public:

	EnemyBehave getState() { return state; }
	virtual void setState(EnemyBehave newState) { state = newState; }
	//ai
public:
	static void cbAniDead(void* parm);
	static void cbAniAttack(void* parm);
};


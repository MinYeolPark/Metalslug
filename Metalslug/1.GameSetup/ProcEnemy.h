#pragma once
#include "ProcObject.h"

enum class EnemyIndex
{
	ArMelee = 0,
	ArBurserker,

	Masknell,
	MTower,
	Kessie,
	EnemyIndexMax,
};

class ProcEnemy
{
public:
	ProcEnemy(int idx);
	virtual ~ProcEnemy();

public:
	iPoint p;
	iSize s;

	bool isActive;

	EnemyIndex idx;

	float up;
	float down;
	bool fall;

	//stats
	int hp;
	int dmg;
	float sight;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;
	iPoint tp;

public:
	virtual void initObj() = 0;
	virtual void updateObj(float dt) = 0;
	virtual void fixedUpdate(float dt) = 0;
	virtual void drawObj(float dt, iPoint off) = 0;
	virtual void freeObj() = 0;

	//ai
public:
	void updateAi(ProcEnemy* e, float dt);

	static void methodDead(void* parm);
	static void methodAttack(void* parm);
};


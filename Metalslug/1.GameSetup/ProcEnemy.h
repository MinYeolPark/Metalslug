#pragma once
#include "ProcObject.h"
#include "ProcPlayer.h"

enum EnemyBehave
{
	eIdleL = 0,
	eIdleR,

	eMoveL,
	eMoveR,

	eDeadL,
	eDeadR,

	eShuffleL,
	eShuffleR,

	ePreAttackL,
	ePreAttackR,

	eAttackL,
	eAttackR,

	EnemyBehaveMax,
};

enum class EnemyIndex
{
	ArMelee = 0,
	ArBurserker,

	Masknell,
	MTower,
	Kessie,
	EnemyIndexMax,
};

class ProcEnemy :
	public ProcObject
{
public:
	ProcEnemy(int idx);
	virtual ~ProcEnemy();

public:
	EnemyIndex idx;
	int hp;
	int dmg;

	float up;
	float down;
	bool fall;

	EnemyBehave state;

	//stats
	float sight;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;
	iPoint tp;

public:
	virtual void initObj();
	virtual void updateObj(float dt);
	virtual void fixedUpdate(float dt);
	virtual void renderObj(float dt, iPoint off);
	virtual void releaseObj();

	//ai
public:
	EnemyBehave getState() { return state; }
	void setState(EnemyBehave eb);
	void updateAi(ProcEnemy* e, float dt);

	bool canAttack() { return (attkRate > _attkRate); }

	static void methodDead(void* parm);
	static void methodAttack(void* parm);

	//Components
public:
	virtual iRect collider();
	virtual iRect attkCollider();
};


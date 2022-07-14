#pragma once
#include "ProcEnemy.h"

enum ArabMeleeBehave
{
	IdleMeleeL = 0,
	IdleMeleeR,

	WalkMeleeL,
	WalkMeleeR,

	DeadMeleeL,
	DeadMeleeR,

	ShuffleMeleeL,
	ShuffleMeleeR,

	AttackMeleeL,
	AttackMeleeR,

	PreAttackMeleeL,
	PreAttackMeleeR,

	FireMeleeL,
	FireMeleeR,

	JumpMeleeL,
	JumpMeleeR,

	MeleeBehaveMax,
};
class ArabMelee :
    public ProcEnemy
{	
public:
	ArabMelee(int index);
	~ArabMelee();

public:
	iImage** imgs;
	iImage* imgCurr;

	int reload;

	Collider* collider;
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();

public:
	virtual void setState(int newState);
	virtual void getDamage(float damage);
};

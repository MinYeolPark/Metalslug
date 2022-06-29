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

	virtual int getFrame();
	virtual bool dead();
public:
	virtual void getDamage(float damage, Collider* c);
	virtual void setState(int newState);
public:
	virtual void update(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};

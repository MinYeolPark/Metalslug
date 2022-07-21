#pragma once
#include "ProcEnemy.h"

enum AbulBehave
{
    IdleAbulL,
    IdleAbulR,

	WalkAbulR,
	WalkAbulL,

    SignalAbulL,
    SignalAbulR,

    SurrenderAbulL,
    SurrenderAbulR,

	AbulBehaveMax,
};
class Abul :
    public ProcEnemy
{
public:
	Abul(int index);
	~Abul();

public:
	iImage** imgs;
	iImage* imgCurr;

public:
	virtual void getDamage(float damage);
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};


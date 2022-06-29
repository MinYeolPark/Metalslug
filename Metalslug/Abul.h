#pragma once
#include "ProcEnemy.h"

enum AbulBehave
{
    IdleAbulL,
    IdleAbulR,

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


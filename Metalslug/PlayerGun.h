#pragma once

#include "iStd.h"

struct GunData
{
	int index;
	int bullet;
	int remain, _remain;
	int ap;
	int speed;
	int ammo;
	float rate, _rate;
};

class ProcObject;
struct PlayerGun
{
	ProcObject* parent;

	int index;
	int bullet;
	int remain, _remain;
	int ap;
	int speed;
	int ammo;
	float rate, _rate;

	PlayerGun(ProcObject* parent);
	virtual ~PlayerGun();

	void init();
	void set(int index, int bullet, int remain, int ap, int speed, int ammo, int _rate, int rate = 0);
	void update(float dt);
	void fire();
};

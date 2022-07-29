#pragma once

#include "iStd.h"
enum GunIndex
{
	Handgun = 0,
	HeavyMachinegun,
	Bomb,

	GunIndexMax,
};
struct Gun
{
	struct GunInfo
	{
		int gunIndex;
		int dmg;
		int speed;
		int remain;
		float rate, _rate;
	};
	GunInfo gi[3] =
	{
		{Handgun, 100, 300, 0},
		{HeavyMachinegun, 100, 150, 200},
		{Bomb, 100, 150, 200},
	};
	Gun()
	{
		this->gunIndex = Handgun;
		this->dmg = (gi + Handgun)->dmg;
		this->speed = (gi + Handgun)->speed;
		this->remain = (gi + Handgun)->remain;
		this->rate = (gi + Handgun)->rate;
	};
	~Gun()
	{

	};
	GunInfo* curGunInfo;
	int gunIndex;
	int dmg;
	int speed;
	int remain;
	float rate, _rate;

	void changeGun(int idx)
	{
		this->gunIndex = idx;
		this->dmg = (curGunInfo + idx)->dmg;
		this->speed = (curGunInfo + idx)->speed;
		this->remain = (curGunInfo + idx)->remain;
		this->rate = (curGunInfo + idx)->rate;
	}
};
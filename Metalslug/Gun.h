#pragma once

#include "iStd.h"
enum GunIndex
{
	Handgun = 0,
	HeavyMachinegun,
	Bomb,
};
struct Gun
{
	struct GunInfo
	{
		GunIndex gunIndex;
		int dmg;
		int speed;
		int remain;
		float rate, _rate;
	};
	GunInfo gi[3] =
	{
		{GunIndex::Handgun, 100, 300, 0},
		{GunIndex::HeavyMachinegun, 100, 150, 200},
		{GunIndex::Bomb, 100, 150, 200},
	};
	Gun()
	{
		this->gunIndex = (gi + 0)->gunIndex;
		this->dmg = (gi + Handgun)->dmg;
		this->speed = (gi + Handgun)->speed;
		this->remain = (gi + Handgun)->remain;
		this->rate = (gi + Handgun)->rate;
		//this->gunIndex = (gi + 1)->gunIndex;
		//this->dmg = (gi + HeavyMachinegun)->dmg;
		//this->speed = (gi + HeavyMachinegun)->speed;
		//this->remain = (gi + HeavyMachinegun)->remain;
		//this->rate = (gi + HeavyMachinegun)->rate;
	};
	~Gun()
	{

	};
	GunInfo* curGunInfo;
	GunIndex gunIndex;
	int dmg;
	int speed;
	int remain;
	float rate, _rate;

	void changeGun(int idx)
	{
		this->gunIndex = (curGunInfo + idx)->gunIndex;
		this->dmg = (curGunInfo + idx)->dmg;
		this->speed = (curGunInfo + idx)->speed;
		this->remain = (curGunInfo + idx)->remain;
		this->rate = (curGunInfo + idx)->rate;
	}
};
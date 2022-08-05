#include "PlayerGun.h"

#include "BulletMgr.h"
#include "ProcBullets.h"

GunData gd[];
PlayerGun::PlayerGun(ProcObject* parent)
{
	this->parent = parent;

	index = 0;
	bullet = 0;
	remain = 0;	_remain = 0;
	ap = 0;
	speed = 0;
	ammo = 0;
	rate = 0;	_rate = 0;
}

PlayerGun::~PlayerGun()
{
}

void PlayerGun::init()
{
	GunData* d = gd;
	set(d->index, d->bullet, d->remain, d->ap, d->speed, d->ammo, d->_rate, d->rate);
}

void PlayerGun::set(int index, int bullet, int remain, int ap, int speed, int ammo, int _rate, int rate)
{
	this->index = index;
	this->bullet = bullet;
	this->remain = remain;
	this->ap = ap;
	this->speed = speed;
	this->ammo = ammo;
	this->rate = rate;
	this->_rate = _rate;
}

void PlayerGun::update(float dt)
{
	if (remain < _remain)
	{
		rate += dt;
		if (rate >= _rate)
		{
			rate -= _rate;
			remain++;
			ammo--;
			fire();
		}
	}
	else
	{
		remain = 0.f;
	}
}

void PlayerGun::fire()
{
	float deg = iPointAngle(iPointMake(1, 0), iPointZero, parent->tp);
	addBullet(parent, bullet, deg);
}

enum PlayerGunIndex
{
	GunHandgun,
	GunHeavyMachinegun,
	
	PlayerGunIndexMax,
};
GunData gd[] =
{
	{Handgun, BulletHandGun, 0, 1, 100, 200, 0, 0, 0.5},
	{HeavyMachinegun, BulletHeavyMachinegun, 0, 3, 100, 220, 200, 0, 0.5},
};
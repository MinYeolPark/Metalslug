#pragma once

#include "iStd.h"
#include "ProcObject.h"

#define pBOT 0
#define pTOP 1
#define WHOLE 2
#define jumpPow			3
#define jumpDecrease	7

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
enum CharacterIndex;
enum PlayerBehave;
class ProcPlayer :
	public ProcObject
{
public:
	ProcPlayer(int index);
	virtual ~ProcPlayer();

public:
	int index;
	iImage** topImgs;
	iImage* topImgCurr;
	iImage** botImgs;
	iImage* botImgCurr;

	Gun* curGun;
	iPoint firePoint;
	iPoint bombPoint;

	float up;
	float down;
	bool fall;

	bool dirRight;			//default Right;
	bool dirUp;

	float fireDeg;
	bool isDead;	
	bool fireing;
	int jumpCombo;

	int maxX;
	int maxY;

	PlayerBehave topState;
	PlayerBehave botState;

	int hp;
	int life;
	float moveSpeed;
	float bombSpeed;
	float attkRange;
	float bombRange;

	int bombs;
	int score;

	//test
	float alpha, alphaDt;

public:
	void fire(iPoint v);
	void bomb(iPoint v);
	void dead();

	void getDamage(float damage, Collider* c);
	void addScore(int score);
	void addBomb(int bomb);
	void changeGun(int index);
public:
	virtual void init(iPoint p);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual bool draw(float dt, iPoint off);
	virtual void free();
};

extern ProcPlayer* player;
enum CharacterIndex
{
	MARCO = 0,
	ERI,
	TAMA,
	PIO,

	CharacterIndexMax,
};
enum PlayerBehave
{
	//===========================================
	//Bot //하체는 종류 4개 끝, 나머지는 TopState로 관리
	//===========================================
	PlayerIdle = 0,
	PlayerWalk,
	PlayerJump,
	PlayerRunJump,
	//===========================================
	//Top
	//===========================================
	PlayerAimtoUp,
	PlayerAimtoNormal,
	PlayerAimtoDown,
	PlayerFire,
	PlayerFireUp,
	PlayerFireDown,
	PlayerMeleeAttack,
	PlayerBomb,
	//===========================================
	// Whole
	//===========================================
	PlayerBrake,
	PlayerCrouch,	
	PlayerCrouchFire,
	//===========================================
	//Only
	//===========================================
	PlayerSpawn,
	PlayerDead,
	PlayerBehaveMax,
};

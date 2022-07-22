#pragma once
#include "iStd.h"

#include "Gun.h"
#include "ProcObject.h"
#define jumpPow			4
#define jumpDecrease	9

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
	float degree;

	Gun* curGun;
	iPoint tp;
	iPoint fp;			//firePoint
	iPoint bombPoint;

	float up;
	float down;
	bool fall;

	bool dirRight;			//default Right;
	bool dirUp;

	float fireDeg;
	bool fireing;
	int jumpCombo;

	float maxX;
	float maxY;

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

	float alpha;
	float inviDt, _inviDt;		//Immortality

	bool canControl;
public:
	void fire(iPoint v);
	void bomb(iPoint v);	

	void getDamage(float damage);
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

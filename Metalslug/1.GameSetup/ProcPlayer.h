#pragma once

#include "ProcObject.h"

#define pBOT 0
#define pTOP 1
#define WHOLE 2
#define jumpPow			8
#define jumpDecrease	20
enum CharacterIndex;
enum PlayerBehave;
class ProcPlayer :
	public ProcObject
{
public:
	ProcPlayer();
	virtual ~ProcPlayer();

public:
	iImage** topImgs;
	iImage* topImgCurr;
	iImage** botImgs;
	iImage* botImgCurr;
		
	int curGun;
	iPoint firePoint;
	int fireDir;

	float up;
	float down;
	bool fall;	

	int jumpCombo;
	float fireRate, _fireRate;	

	PlayerBehave topState;
	PlayerBehave botState;

	int hp;
	int life;
	float moveSpeed;
	int bombs;
	int score;
public:
	void setState(PlayerBehave pb, iPoint v);

public:
	void jump();
	void fire();
public:
	virtual void initObj() override;
	virtual void updateObj(float dt) override;
	virtual void fixedUpdate(float dt) override;
	virtual void drawObj(float dt, iPoint off) override;
	virtual void freeObj() override;

public:
	//virtual iRect collider();
};

enum CharacterIndex
{
	PIO = 0,
	ERI,
	MARCO,
	TAMA,

	PlayerIndexMAX
};
enum PlayerBehave
{
	//===========================================
	//Bot //하체는 종류 4개 끝, 나머지는 TopState로 관리
	//===========================================

	IdleR = 0,
	IdleL,

	WalkR,
	WalkL,

	JumpR,
	JumpL,

	RunJumpR,
	RunJumpL,

	//===========================================
	//Top
	//===========================================
	FireR,
	FireL,

	//LookUpR = 100,
	//LookUpL,


	////===========================================
	////Whole
	////===========================================
	//CrouchR = 200,
	//CrouchL,

	//CrouchMoveLeft,
	//CrouchMoveRight,

	//BrakeR,
	//BrakeL,

	//DeadR,
	//DeadL,

	PlayerBehaveMax,
};

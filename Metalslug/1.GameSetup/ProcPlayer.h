#pragma once

#include "ProcObject.h"

#define pBOT 0
#define pTOP 1
#define WHOLE 2
#define jumpPow			3
#define jumpDecrease	7

struct Gun
{	
	enum GunIndex
	{
		Handgun = 0,
		HeavyMachinegun,
		Bomb,
	};
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
		{GunIndex::Handgun, 100, 100, 0},
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
	ProcPlayer();
	virtual ~ProcPlayer();

public:
	iImage** topImgs;
	iImage* topImgCurr;
	iImage** botImgs;
	iImage* botImgCurr;
		
	Gun* curGun;
	iPoint firePoint;
	iPoint targetPoint;
	iPoint v;

	float up;
	float down;
	bool fall;	

	bool isRight;				//check is Right.
	bool isAttacking;
	bool isAimup;
	bool isCrouching;
	bool isAimDown;

	int jumpCombo;

	PlayerBehave topState;
	PlayerBehave botState;

	int hp;
	int life;
	float moveSpeed;
	float bombSpeed;
	float attkRange;

	int bombs;
	int score;
public:
	void setTopState(PlayerBehave pb, iPoint v);
	void setBotState(PlayerBehave pb, iPoint v);
public:
	void jump(iPoint v);
	void crouch();
	void aimUp();
	void fire(iPoint v);
	void bomb(iPoint v);
public:
	virtual void initObj() ;
	virtual void updateObj(float dt);
	virtual void fixedUpdate(float dt) ;
	virtual bool drawObj(float dt, iPoint off) ;
	virtual void freeObj() ;

public:
public:
	iRect collider();

public:
	static void cbAniJump(void* parm);
	static void cbAniBrake(void* parm);
	static void cbAniFire(void* parm);

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

	AimtoUpR,
	AimtoUpL,

	AimtoNormR,
	AimtoNormL,

	AimDownR,
	AimDownL,

	FireR,
	FireL,

	MeleeR,
	MeleeL,

	BombR,
	BombL,

	/// 
	/// Whole
	/// 
	SpawnR,
	SpawnL,

	CrouchR,
	CrouchL,

	BrakeR,
	BrakeL,


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



enum RecordKind
{
	RecordStep = 0,



};

#define recordMax 1000
#define recordDt 1.0f
extern Texture** texRecord;

struct Record
{
	RecordKind index;
	iPoint position;
	float remindDt;

	void set(RecordKind index, iPoint p)
	{
		this->index = index;
		position = p;
		remindDt = recordDt;
	}

	bool paint(float dt, iPoint off)
	{
		remindDt -= dt;
		if (remindDt < 0.0f)
			return true;

		float a = 1.0f - remindDt / recordDt;
		setRGBA(1, 1, 1, a);

		iPoint p = position + off;
		drawImage(texRecord[index], p.x, p.y, VCENTER | HCENTER);

		return false;
	}
};

void loadRecord();
void freeRecord();
void drawRecord(float dt, iPoint off);
void addRecord(RecordKind index, iPoint p);

extern float fireDegree;

void loadFB();
void freeFB();
void drawFB(float dt, iPoint off);
void addFB(int index, iPoint p, float degree);

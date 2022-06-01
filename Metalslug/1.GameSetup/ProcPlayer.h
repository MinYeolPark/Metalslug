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
	void setTopState(PlayerBehave pb, iPoint v);
	void setBotState(PlayerBehave pb, iPoint v);
public:
	void jump();
	void fire(iPoint v);
public:
	virtual void initObj() override;
	virtual void updateObj(float dt) override;
	virtual void fixedUpdate(float dt) override;
	virtual void drawObj(float dt, iPoint off) override;
	virtual void freeObj() override;

public:
public:
	iRect collider();

public:
	static void cbAniFire(void* parm);

};

extern ProcPlayer* player;
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

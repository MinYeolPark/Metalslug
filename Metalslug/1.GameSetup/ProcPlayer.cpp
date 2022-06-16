#include "ProcPlayer.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "BulletMgr.h"
#include "InputMgr.h"
#include "EffectMgr.h"

#include "Proc.h"
#include "ProcField.h"


#include "Item.h"
ProcPlayer* player;
ImageInfo topImageInfo[];
ImageInfo botImageInfo[];
ProcPlayer::ProcPlayer()
{
	layer = ObjLayer::Player;
	p = iPointZero;
	s = iSizeZero;
	isActive = false;

	topImgs = NULL;
	topImgCurr = NULL;
	botImgs = NULL;
	botImgCurr = NULL;

	curGun = new Gun();//{ HandGun, 100, 100, 0 };
	//firePoint
	up = 0;
	down = 0;
	fall = true;

	isAttacking = false;
	isAimup = false;
	isCrouching = false;
	isAimDown = false;

	hp = 100;
	life = 2;
	moveSpeed = 150.f;
	bombSpeed = 10.f;
	attkRange = 40;

	bombs = 20;
	score = 0;

	topState = IdleR;
	botState = IdleR;
	topImgs = createImgPlayer(topImageInfo, 13 * 2 , this);
	botImgs = createImgPlayer(botImageInfo, 4 * 2, this);

	loadFB();
}

ProcPlayer::~ProcPlayer()
{
	delete curGun;
	delete player;
}

void ProcPlayer::initObj()
{
	this->isActive = true;
	topState = SpawnR;
	botState = IdleR;
	this->p = iPointMake(100, 200);
}

void ProcPlayer::updateObj(float dt)
{
	v = iPointZero;
	firePoint = iPointMake(p.x, p.y - 20);
	if (getKeyStat(keyboard_left))
		v.x = -1;
	else if (getKeyStat(keyboard_right))	
		v.x = 1;
	if (getKeyStat(keyboard_up))
		v.y = -1;
	else if (getKeyStat(keyboard_down))
		v.y = 1;
	
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		v *= (moveSpeed * dt);

		if (v.x < 0)
			isRight = false;
		else if (v.x > 0)
			isRight = true;

		if (!up)
		{
			if (v.x>0)
			{
				setBotState(WalkR, v);
				if (!isAttacking)
					setTopState(WalkR, v);
			}
			else if(v.x<0)// if (v.x < 0)
			{
				setBotState(WalkL, v);
				if (!isAttacking)
					setTopState(WalkL, v);
			}
		}
	}
	else//
	{
		if (topState == WalkR || topState == WalkL)
			setTopState((PlayerBehave)(BrakeR + topState % 2), v);
	}
	
	if (getKeyDown(keyboard_z))
		jump(v);

	if (getKeyDown(keyboard_up))
		aimUp();
	else if (getKeyUp(keyboard_up))
		setTopState((PlayerBehave)(IdleR + topState % 2), v);

	if (getKeyDown(keyboard_down))
		crouch();
	else if(getKeyUp(keyboard_down))
		setTopState((PlayerBehave)(IdleR + topState % 2), v);

	p.x += v.x;

	fixedUpdate(dt);
}
void ProcPlayer::fixedUpdate(float dt)
{
#if 1
	//Camera Move
	float x = p.x + bg->off.x;
	if (x < devSize.width / 3)
	{
		bg->move(iPointMake(devSize.width / 3 - x, 0));
	}
	else if (x > devSize.width * 2 / 3)
	{
		bg->move(iPointMake(devSize.width * 2 / 3 - x, 0));
	}
#endif	
	int maxY = *(bg->maxY + (int)p.x);
	if (p.y > maxY)		//on air
	{
		p.y = maxY;
		up = 0;
		down = 0;
		fall = false;
		jumpCombo = 0;
	}
	else
		fall = true;

	if (up)
	{
		p = (iPointMake(p.x, p.y -= jumpPow));
		up += jumpDecrease * dt;

		if (up > 0.0f)
			up = 0.0f;
	}

	if (fall)
	{
		if (p.y < maxY)
		{
			down += jumpDecrease * dt;
			p = (iPointMake(p.x, p.y += down));
		}
	}

	//Check Item collider
	if (containPoint(p, item->collider()))
	{
		//if item is gun
		//curGun->changeGun();
	}
}
bool ProcPlayer::drawObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);

	botImgCurr = botImgs[botState];
	topImgCurr = topImgs[topState];

	if (topState < SpawnR)
		botImgCurr->paint(dt, p + off);
	topImgCurr->paint(dt, p + off);
	{
#ifdef _DEBUG
			setRGBA(1, 0, 1, 0.5);
			setDotSize(10);
			drawDot(p + off);
#endif
	}

	//addRecord(RecordStep, p);
	//drawRecord(dt, off);
	setRGBA(1, 1, 1, 1);
#if 0
	if (getKeyDown(keyboard_space))
		addFB(0, p, fireDegree);
	else if (getKeyStat(keyboard_up))
	{
		if (fireDegree < 90)
		{
			fireDegree += (90 / 0.5f * dt);
			if (fireDegree > 90)
				fireDegree = 90;
		}
		else if( fireDegree > 90 )
		{
			fireDegree -= (90 / 0.5f * dt);
			if (fireDegree < 90)
				fireDegree = 90;
		}
	}
	drawFB(dt, off);
#endif
	return !isActive;
}

void ProcPlayer::freeObj()
{
}

void ProcPlayer::setTopState(PlayerBehave pb, iPoint v)
{
	if (pb == JumpR || pb == JumpL)
	{
		topImgs[pb]->startAnimation(cbAniJump, this);
		botImgs[pb]->startAnimation(cbAniJump, this);
	}
	if (pb == RunJumpR || pb == RunJumpL)
	{
		topImgs[pb]->startAnimation(cbAniJump, this);
		botImgs[pb]->startAnimation(cbAniJump, this);
	}
	if (pb == AimtoUpR || pb == AimtoUpL)
	{
		topImgs[pb]->startAnimation();
	}
	if (pb == AimtoNormR || pb == AimtoUpL)
	{
		topImgs[pb]->startAnimation();
	}
	if (pb == FireR || pb == FireL)
	{
		//#issue
		topImgCurr[FireR].frame;//bottom sync

		topImgs[pb]->startAnimation(cbAniFire, this);
	}
	if (pb == MeleeR || pb == MeleeL)
	{
		topImgs[pb]->startAnimation(cbAniFire, this);
	}
	if (pb == BombR || pb == BombL)
	{
		topImgs[pb]->startAnimation(cbAniFire, this);
	}
	if (pb == SpawnR || pb == SpawnL)
	{
		topImgs[pb]->startAnimation();
	}
	if (pb == CrouchR || pb == CrouchL)
	{
		topImgs[pb]->startAnimation();
	}
	if (pb == BrakeR || pb == BrakeL)
	{
		topImgs[pb]->startAnimation(cbAniBrake, this);
	}
	topState = pb;
}

void ProcPlayer::setBotState(PlayerBehave pb, iPoint v)
{
	botState = pb;

}

void ProcPlayer::jump(iPoint v)
{
	if (jumpCombo == 1)
		return;
	if (v.x > 0)
	{
		setTopState(RunJumpR, v);
		setBotState(RunJumpR, v);
	}
	else if (v.x < 0)
	{
		setTopState(RunJumpL, v);
		setBotState(RunJumpL, v);
	}
	else
	{
		setTopState((PlayerBehave)(JumpR + topState % 2), v);
		setBotState((PlayerBehave)(JumpR + botState % 2), v);
	}

	up -= jumpPow;
	jumpCombo++;	
}

void ProcPlayer::crouch()
{
	setTopState((PlayerBehave)(CrouchR + topState % 2), v);
	setBotState((PlayerBehave)(IdleR + botState % 2), v);
}

void ProcPlayer::aimUp()
{
	isAimup = !isAimup;
	
	setTopState((PlayerBehave)(AimtoUpR + topState % 2), v);
	setBotState((PlayerBehave)(IdleR + botState % 2), v);
}

void ProcPlayer::fire(iPoint v)
{	
	isAttacking = true;
	ProcEnemy* eNear = NULL;
	float dNear = 0xffffff;
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];
		float d = iPointLength(p - e->p);
		if (attkRange > d)
		{
			dNear = d;
			if (e->getState() != (EnemyBehave)(DeadEnemyL + e->state % 2))
				eNear = e;			
		}
	}

	if (eNear)
	{
		setTopState((PlayerBehave)(MeleeR + topState % 2), v);
		eNear->hp -= 100;
		if (eNear->hp <= 0)
			eNear->setState((EnemyBehave)(DeadEnemyL + eNear->state % 2));
	}
	else
	{
		setTopState((PlayerBehave)(FireR + topState % 2), v);		
		addBullet(this, 0, fireDegree);
	}

	if (v.x > 0)
		setBotState(WalkR, v);
	else if (v.x < 0)
		setBotState(WalkL, v);
	else
		setBotState((PlayerBehave)(IdleR + botState % 2), v);
}

void ProcPlayer::bomb(iPoint v)
{
	setTopState((PlayerBehave)(BombR + topState % 2), v);

	addBullet(this, curGun->Bomb, fireDegree);
}

iRect ProcPlayer::collider()
{
	return iRectMake(p.x - 20 + bg->off.x, p.y - 70 + bg->off.y, 40, 70);
}
void ProcPlayer::cbAniJump(void* parm)
{
	ProcPlayer* pp = (ProcPlayer*)parm;
	pp->setTopState((PlayerBehave)(WalkR + pp->topState % 2), iPointZero);
	pp->setBotState((PlayerBehave)(WalkR + pp->botState % 2), iPointZero);
}

void ProcPlayer::cbAniBrake(void* parm)
{
	ProcPlayer* pp = (ProcPlayer*)parm;
	pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
	pp->setBotState((PlayerBehave)(IdleR + pp->botState % 2), iPointZero);
}

void ProcPlayer::cbAniFire(void* parm)
{
	ProcPlayer* pp = (ProcPlayer*)parm;	
	pp->isAttacking = false;
	pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
}


ImageInfo botImageInfo[] =
{
	{
		"assets/Player/Bot_Idle_%02d.png",
		1, 1.0f, { -20 / 2, 0 },
		0.1f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Bot_Walk_%02d.png",
		12, 1.0f, { -20 / 2, 0 },
		0.1f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Bot_Jump_%02d.png",
		6, 1.0f, { -22 / 2, 0 },
		0.1f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Bot_RunJump_%02d.png",
		6, 1.0f, { -22 / 2, 0 },
		0.1f,
		1,
		{255,0,0,255},
		NULL,
	},
};
ImageInfo topImageInfo[] =
{
	{
		"assets/Player/Top_Idle_%02d.png",
		4, 1.0f, { -26 / 2, 10 },		
		0.1f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Top_Walk_%02d.png",
		12, 1.0f, { -18 / 2, 12 },
		0.1f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Top_Jump_%02d.png",
		6, 1.0f, {  -26 / 2, 18 },
		0.1f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Top_RunJump_%02d.png",
		6, 1.0f, { -26 / 2, 6 },
		0.1f,
		0,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/AimUp_%02d.png",
		6, 1.0f, { -32 / 2, 16 },
		0.06f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/AimtoNorm_%02d.png",
		4, 1.0f, { -32 / 2, 16 },
		0.06f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/Player/AimDown_%02d.png",
		3, 1.0f, { -20 / 2, 16 },
		0.06f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Top_Fire_%02d.png",
		9, 1.0f, { -20 / 2, 16 },
		0.06f,
		1,
		{255,0,0,255},
		ProcPlayer::cbAniFire,
	},
	{
		"assets/Player/Melee_%02d.png",
		6, 1.0f, { -28 / 2, 6 },
		0.06f,
		1,
		{255,255,255,255},
		ProcPlayer::cbAniFire,
	},
	{
		"assets/Player/Bomb_%02d.png",
		6, 1.0f, { -32 / 2, 8 },
		0.06f,
		1,
		{255,0,0,255},
		ProcPlayer::cbAniFire,
	},
	{
		"assets/Player/Spawn_%02d.png",
		7, 1.0f, { -29 / 2, 0 },
		0.04f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Crouch_%02d.png",
		11, 1.0f, { -51 / 2, 0 },
		0.06f,
		1,
		{255,0,0,255},
		NULL,
	},
	{
		"assets/Player/Brake_%02d.png",
		4, 1.0f, { -20 / 2, 0 },
		0.12f,
		1,
		{255, 255, 255, 255},
		ProcPlayer::cbAniBrake,
	},
};


#if 1
Texture** texRecord;
Record* _record;
Record** record;
int recordNum;

void loadRecord()
{
	texRecord = new Texture * [1];
	texRecord[0] = createImage("assets/ .png");

	_record = new Record[recordMax];
	memset(_record, 0x00, sizeof(Record) * recordMax);
	record = new Record*[recordMax];
	recordNum = 0;
}

void freeRecord()
{
	for (int i = 0; i < 1; i++)
		freeImage(texRecord[i]);
	delete texRecord;

	delete _record;
	delete record;
}

void drawRecord(float dt, iPoint off)
{
	for (int i = 0; i < recordNum; i++)
	{
		if (record[i]->paint(dt, off))
		{
			recordNum--;
			record[i] = record[recordNum];
			i--;
		}
	}
	setRGBA(1, 1, 1, 1);
}

static iPoint prevRecordPosition;
void addRecord(RecordKind index, iPoint p)
{
	if (prevRecordPosition == p)
		return;

	for (int i = 0; i < recordMax; i++)
	{
		Record* r = &_record[i];
		if (r->remindDt > 0.0f) continue;

		r->set(index, p);

		record[recordNum] = r;
		recordNum++;
		return;
	}
}

float fireDegree;
Texture** texsFB;

struct FB
{
	bool alive;
	int index;
	iPoint position, v;
	float degree;

	void set(int index, iPoint p, float degree)
	{
		alive = true;
		this->index = index;
		position = p;
		this->degree = degree;

		v = iPointRotate(iPointMake(1, 0), iPointZero, degree) * 300;
	}

	bool paint(float dt, iPoint off)
	{
		Texture* t = texsFB[index];
		position += v * dt;
		iPoint p = position + off;
		drawImage(t, p.x, p.y, 1.0f, 1.0f,
			VCENTER | HCENTER, 0, 0, t->width, t->height, 2, degree);

		alive = containPoint(p, iRectMake(-20, -20, devSize.width + 40, devSize.height + 40));
		return !alive;
	}
};

FB* _fb;
FB** fb;
int fbNum;
#define fbMax 100

void loadFB()
{
	fireDegree = 0;

	texsFB = new Texture * [1];
	texsFB[0] = createImage("assets/Bullets/HeavyMachineGun_00.png");

	_fb = new FB[fbMax];
	memset(_fb, 0x00, sizeof(FB) * fbMax);
	fb = new FB * [fbMax];
	fbNum = 0;
}
void freeFB()
{
	for (int i = 0; i < 1; i++)
		freeImage(texsFB[i]);
	delete texsFB;

	delete _fb;
	delete fb;
}
void drawFB(float dt, iPoint off)
{
	for (int i = 0; i < fbNum; i++)
	{
		if (fb[i]->paint(dt, off))
		{
			fbNum--;
			fb[i] = fb[fbNum];
			i--;
		}
	}
}
void addFB(int index, iPoint p, float degree)
{
	for (int i = 0; i < fbMax; i++)
	{
		FB* f = &_fb[i];
		if (f->alive == false)
		{
			f->set(index, p, degree);
			fb[fbNum] = f;
			fbNum++;
			return;
		}
	}
}
#endif
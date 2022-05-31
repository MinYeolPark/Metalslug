#include "ProcPlayer.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "InputMgr.h"

#include "Proc.h"
#include "ProcField.h"
#include "ProcBullets.h"

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

	up = 0;
	down = 0;
	fall = true;
	
	_fireRate = 0.05f;
	fireRate = 0.0f;

	hp=100;
	life=2;
	moveSpeed = 300.f;
	bombs=20;
	score=0;

	topState = IdleR;
	botState = IdleR;
	topImgs = createImgPlayer(topImageInfo, this);
	botImgs = createImgPlayer(botImageInfo, this);

	loadRecord();
}

ProcPlayer::~ProcPlayer()
{
	delete player;

	freeRecord();
}

void ProcPlayer::initObj()
{
	this->isActive = true;

	this->p = iPointMake(100, 300);
}

void ProcPlayer::updateObj(float dt)
{
	iPoint v = iPointZero;
	if (v != iPointZero)
		v /= iPointLength(v);

	if (getKeyStat(keyboard_left))
		v.x = -1;
	else if (getKeyStat(keyboard_right))
		v.x = 1;
	if (getKeyStat(keyboard_up))
		v.y = -1;
	else if (getKeyStat(keyboard_down))
		v.y = 1;

	if (up)
	{
		if (v.x > 0)
		{
			setBotState(RunJumpR, v);
			setTopState(RunJumpR, v);
		}
		else if (v.x < 0)
		{
			setBotState(RunJumpL, v);
			setTopState(RunJumpL, v);
		}
		else
		{
			setBotState((PlayerBehave)(JumpR + topState % 2), v);
			setTopState((PlayerBehave)(JumpR + botState % 2), v);			
		}
	}
	/// <summary>
	/// //////////////////
	/// </summary>
	/// <param name="dt"></param>
	if (v.x > 0)
	{
		if (up)
		{
			setBotState(RunJumpR, v);
			setTopState(RunJumpR, v);
		}
		else
		{
			setBotState(WalkR, v);
			setTopState(WalkR, v);
		}
	}
	else if (v.x < 0)
	{
		if (up)
		{
			setTopState(RunJumpL, v);
			setBotState(RunJumpL, v);			
		}
		else
		{
			setBotState(WalkL, v);
			setTopState(WalkL, v);
		}
	}
	else//v==iPointzero
	{		
		if (topState < FireR)
		{
			setTopState((PlayerBehave)(IdleR + topState % 2), v);
			setBotState((PlayerBehave)(IdleR + botState % 2), v);			
		}
	}

	if (getKeyStat(keyboard_z))
		jump();
	if (getKeyStat(keyboard_x))
		fire(v);
	v *= moveSpeed * dt;
	p += v;

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
#if 1
	//Camera fix
	//if (x > bg->eventTrigger.x)
		//printf("event triggered");

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
	ProcObject::fixedUpdate(dt);
}
void ProcPlayer::drawObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);

	if (isActive)
	{
		botImgCurr = botImgs[botState];
		topImgCurr = topImgs[topState];

		if(botState<FireR)
			botImgCurr->paint(dt, p + off);	
		topImgCurr->paint(dt, p + off);
	}
	{
#ifdef _DEBUG
			setRGBA(1, 0, 1, 0.5);
			setDotSize(10);
			drawDot(p + off);
			drawDot(p.x + off.x, p.y - 10 + off.y);
#endif
	}

	addRecord(RecordStep, p);
	drawRecord(dt, off);
	setRGBA(1, 1, 1, 1);
}

void ProcPlayer::freeObj()
{
}

void ProcPlayer::setTopState(PlayerBehave pb, iPoint v)
{
	topState = pb;
		
	if (pb == FireR || pb == FireL)
	{
		topImgs[pb]->startAnimation(cbAniFire, this);
	}
}

void ProcPlayer::setBotState(PlayerBehave pb, iPoint v)
{
	botState = pb;

}

void ProcPlayer::jump()
{
	if (jumpCombo == 1)
		return;

	up -= jumpPow;
	jumpCombo++;
}

void ProcPlayer::fire(iPoint v)
{	
	setTopState((PlayerBehave)(FireR + topState % 2), v);
}

void ProcPlayer::cbAniFire(void* parm)
{
	printf("cbAniFire\n");

	ProcPlayer* pp = (ProcPlayer*)parm;	
	pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
}

ImageInfo botImageInfo[] =
{
	{
		"assets/Player/Bot_Idle_%02d.png",
		1, 1.0f, { -20 / 2, 0 },
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Bot_Walk_%02d.png",
		12, 1.0f, { -22 / 2, 0 },
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Bot_Jump_%02d.png",
		6, 1.0f, { -22 / 2, 0 },
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Bot_RunJump_%02d.png",
		6, 1.0f, { -22 / 2, 0 },
		0.1f,
		0,
		{255,255,255,255},
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
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Top_Walk_%02d.png",
		12, 1.0f, { -18 / 2, 12 },
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Top_Jump_%02d.png",
		6, 1.0f, {  -26 / 2, 18 },
		0.1f,
		0,
		{255,255,255,255},
		NULL,
	},
	{
		"assets/Player/Top_RunJump_%02d.png",
		6, 1.0f, { -26 / 2, 6 },
		0.1f,
		0,
		{255,255,255,255},
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
};

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

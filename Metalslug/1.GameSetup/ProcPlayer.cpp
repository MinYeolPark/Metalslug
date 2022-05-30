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
	moveSpeed = 100.f;
	bombs=20;
	score=0;

	topState = IdleR;
	botState = IdleR;
	topImgs = createImgCharReverse(topImageInfo, this);
	botImgs = createImgCharReverse(botImageInfo, this);
}

ProcPlayer::~ProcPlayer()
{
	delete player;
}

void ProcPlayer::initObj()
{
	this->isActive = true;

	this->p = iPointMake(100, 300);
}

void ProcPlayer::updateObj(float dt)
{
	iPoint v = iPointZero;
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
		v *= moveSpeed * dt;

		if (!up)
		{
			if (v.x > 0)
				setState(WalkR, v);
			else if (v.x < 0)
				setState(WalkL, v);
		}

		p += v * moveSpeed * dt;
	}
	


	fixedUpdate(dt);
}
void ProcPlayer::fixedUpdate(float dt)
{
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
		topImgCurr = topImgs[topState];
		botImgCurr = botImgs[botState];
		
		topImgCurr->paint(dt, p + off);
		botImgCurr->paint(dt, p + off);	
	}
	{
		#ifdef _DEBUG
			setRGBA(1, 0, 1, 0.5);
			setDotSize(10);
			drawDot(p + off);
		
#if 0
			setRGBA(1, 1, 1, 1);
			setStringName("assets/BMJUA_ttf.ttf");
			setStringSize(30);
			setStringRGBA(1, 0, 1, 1);
			setStringBorder(0);
			drawString(10, 200, TOP | LEFT, "%d", jumpCombo);
		#endif
#endif
	}
	setRGBA(1, 1, 1, 1);
}

void ProcPlayer::freeObj()
{
}

void ProcPlayer::setState(PlayerBehave pb, iPoint v)
{
	topState = pb;
	botState = pb;
}


ImageInfo botImageInfo[] =
{
	{
		"assets/Player/Bot_Idle_%02d.png",
		1, 2.0f, { -20 / 2, -24 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Bot_Walk_%02d.png",
		1, 2.0f, { -22 / 2, -26 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Bot_Jump_%02d.png",
		1, 2.0f, { -22 / 2, -30 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Bot_RunJump_%02d.png",
		1, 2.0f, { -22 / 2, -30 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
};
ImageInfo topImageInfo[] =
{
	{
		"assets/Player/Top_Idle_%02d.png",
		4, 2.0f, { -32 / 2, -28 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Top_Walk_%02d.png",
		12, 2.0f, { -32 / 2, -28 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Top_Jump_%02d.png",
		6, 2.0f, { -29 / 2, -26 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
	{
		"assets/Player/Top_RunJump_%02d.png",
		6, 2.0f, { -33 / 2, -33 },
		0.18f,
		0,
		iColor4fMake(255,255,255,255),
		NULL,
	},
};
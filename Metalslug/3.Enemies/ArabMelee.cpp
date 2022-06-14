#include "ArabMelee.h"

#include "ImgMgr.h"
ImageInfo imgMeleeInfo[];
static iImage** imgEnemy = NULL;
ArabMelee::ArabMelee(int idx) : ProcEnemy(idx)
{
	idx = (int)EnemyIndex::ArMelee;

	speed = 80;
	up = 0.f;
	down = 0.f;
	fall = false;

	hp = 100;
	dmg = 100;
	sight = 100;
	attkRange = 50;
	attkRate = 0.f;
	_attkRate = 2.f;
	aiDt = 0.f;
	_aiDt = 2.f;
	tp = iPointZero;

	imgs = NULL;
	imgCurr = NULL;
	
	state = EnemyBehave::IdleEnemyL;

	if( imgEnemy==NULL )
		imgEnemy = createImgEnemyReverse(imgMeleeInfo, EnemyBehaveMax, this);

	imgs = new iImage * [EnemyBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * EnemyBehaveMax);
	for (int i = 0; i < EnemyBehaveMax; i++)
		imgs[i] = imgEnemy[i]->clone();
}

ArabMelee::~ArabMelee()
{
	for (int i = 0; i < EnemyBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

void ArabMelee::initObj()
{
}

void ArabMelee::updateObj(float dt)
{
	//Checking Dead
	if (state == DeadEnemyL || state == DeadEnemyR)
	{
		state = (EnemyBehave)(DeadEnemyL + state % 2);
		return;
	}
	float len = iPointLength(player->p - p);
	if (len < sight)
		tp = player->p;
#if 1
	if (tp != iPointZero)
	{
		iPoint v = tp - p;
		v /= iPointLength(v);

		if (v.x > 0)
		{
			state = WalkEnemyR;
		}
		else if (v.x < 0)
		{
			state = WalkEnemyL;
		}
		else
			state = (EnemyBehave)(IdleEnemyL + state % 2);
	}	
#endif

#if 1
	if (!fall)
	{
		if (tp != iPointZero)
		{
#if 1
			if (movePoint(p, p, tp, speed * dt))
#else
			iPoint p0 = p;
			float moveDistance = speed * dt;
			bool arrive = movePoint(p, p, tp, moveDistance);
			iPoint p1 = p;
			p1.y = *(bg->maxY + (int)p.x);

			iPoint v = p1 - p0;
			if (v != iPointZero)
				v /= iPointLength(v);
			p = p0 + v * moveDistance;

			if (arrive)
#endif
			{
				float dp = 0xffffff;
				if (containPoint(p, player->collider()))
				{
					float d = iPointLength(p - player->p);
					if (dp > d)
					{
						dp = d;
					}
				}
			}
		}
	}
#endif
	p.y = *(bg->maxY + (int)p.x);
	return;
	fixedUpdate(dt);
}

void ArabMelee::fixedUpdate(float dt)
{
	int maxY = *(bg->maxY + (int)p.x);
	if (p.y >= maxY)
	{
		up = 0;
		down = 0;
		fall = false;

		p.y = maxY;
	}
	else
		fall = true;

	if (fall)
	{
		if (p.y < maxY)
		{
			down += jumpDecrease * dt;
			p = (iPointMake(p.x, p.y += down));
		}
	}

	if (tp != iPointZero)
		tp.y = maxY;
}

void ArabMelee::drawObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(collider());
#endif // DEBUG
	setRGBA(1, 1, 1, 1);
}

void ArabMelee::freeObj()
{
	//#issue한번만 지우기
	if (imgEnemy != NULL)
	{
		for (int i = 0; i < EnemyBehaveMax; i++)
			delete imgEnemy[i];
		delete imgEnemy;
		imgEnemy = NULL;
	}
}

ImageInfo imgMeleeInfo[] =
{
	{
		"assets/ArabMelee/ArabMelee_Idle_%02d.png",
		9, 1.0f, { -36, 0},
		0.18f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_Move_%02d.png",
		12, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/Arab_Melee_Dead_%02d.png",
		20, 1.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		ProcEnemy::methodDead,
	},
	{
		"assets/ArabMelee/ArabMelee_Shuffle_%02d.png",
		6, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_AttackPre_%02d.png",
		4, 2.0f, { -36, 0},
		0.18f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_AttackMelee_%02d.png",
		8, 2.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
};

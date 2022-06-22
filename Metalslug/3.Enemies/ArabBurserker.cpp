#include "ArabBurserker.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
ImageInfo imgBurserkInfo[];
static iImage** imgEnemy = NULL;
ArabBurserker::ArabBurserker(int idx) : ProcEnemy(idx)
{
	idx = (int)EnemyIndex::IdxArMelee;
	state = IdleEnemyL;

	ai = AI::enemyAI0;

	up = 0.f;
	down = 0.f;
	fall = false;

	hp = 100;
	dmg = 100;
	sight = 200;
	moveSpeed = 200;
	attkRange = 70;
	attkRate = 0.f;
	_attkRate = 2.f;
	aiDt = 0.f;
	_aiDt = 2.f;
	tp = iPointMake(-1, -1);

	imgs = NULL;
	imgCurr = NULL;

	state = EnemyBehave::IdleEnemyL;

	if (imgEnemy == NULL)
		imgEnemy = createImgReverse(imgBurserkInfo, EnemyBehaveMax, this);

	imgs = new iImage * [EnemyBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * EnemyBehaveMax);
	for (int i = 0; i < EnemyBehaveMax; i++)
		imgs[i] = imgEnemy[i]->clone();
}

ArabBurserker::~ArabBurserker()
{
	for (int i = 0; i < EnemyBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

void ArabBurserker::setState(EnemyBehave newState)
{
	state = newState;

	if (state == AttackEnemyL || state == AttackEnemyR)
	{
		imgs[newState]->startAnimation(ProcEnemy::cbAniAttack, this);
	}
}

void ArabBurserker::initObj()
{
	ai = AI::enemyAIBurserker;
}

void ArabBurserker::initObj(iPoint v)
{
	ai = AI::enemyAIBurserker;
	this->v = v;
	//tp = { p.x - 100, p.y };
}
void ArabBurserker::updateObj(float dt)
{
	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		ai(this, dt);
	}

	if (tp != iPointMake(-1, -1))
	{
		if (movePoint(p, p, tp, moveSpeed * dt))
		{
			
		}
	}
	p.y = *(bg->maxY + (int)p.x);
	return;
	fixedUpdate(dt);
}

void ArabBurserker::fixedUpdate(float dt)
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

bool ArabBurserker::drawObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	//drawDot(p + off);
	//drawRect(collider());
#endif // DEBUG
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabBurserker::freeObj()
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

ImageInfo imgBurserkInfo[] =
{
	{
		"assets/ArabBurserker/Arab_Bur_Idle_%02d.png",
		4, 1.0f, { -36, 0},
		0.18f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabBurserker/Arab_Bur_Walk_%02d.png",
		6, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabBurserker/Arab_Bur_Dead_%02d.png",
		8, 1.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		ProcEnemy::cbAniDead,
	},
};

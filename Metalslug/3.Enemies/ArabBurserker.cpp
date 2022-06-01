#include "ArabBurserker.h"

#include "EnemyMgr.h"

static EnemyImageInfo imageInfo[3] =
{
	{
		"Resources/Character/Enemies/Arab_Bur/Arab_Bur_Idle_%02d.png",
		4, 2.0f, { -28, -56},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/Arab_Bur/Arab_Bur_Walk_%02d.png",
		6, 2.0f, { -28, -56},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/Arab_Bur/Arab_Bur_Dead_%02d.png",
		9, 2.0f, { -28, -56},
		0.15f,
		1,
		ProcEnemy::methodDead,
	},
};

static iImage** imgEnemy = NULL;
ArabBurserker::ArabBurserker(int idx): ProcEnemy(idx)
{
	//stats init
	moveSpeed = 150;
	sight = 200;
	attkRange = 0;
	attkRate = 0.f, _attkRate = 2.f;
	aiDt = 0.f, _aiDt = 2.f;

	if (imgEnemy == NULL)
		imgEnemy = createImgCharReverse(imageInfo, this);

	imgs = new iImage * [6];
	memset(imgs, 0x00, sizeof(iImage*) * 6);
	for (int i = 0; i < 6; i++)
		imgs[i] = imgEnemy[i]->clone();
}

ArabBurserker::~ArabBurserker()
{
	for (int i = 0; i < 6; i++)
	{
		if (imgs[i])
			delete imgs[i];
	}
	delete imgs;	
}

void ArabBurserker::initObj()
{
}

void ArabBurserker::updateObj(float dt)
{
	ProcEnemy::updateObj(dt);

	iPoint v = player->p - p;
	v /= iPointLength(v);
	if (v.x > 0)
	{
		//setState((EnemyBehave)(eMoveR));
		//tp.x = bg->maxW + bg->off.x + 20;
	}
	else
	{
		//setState((EnemyBehave)(eMoveL));
		//tp.x = bg->off.x + 20;
	}

	int d = getDistance(p, tp);
}

void ArabBurserker::fixedUpdate(float dt)
{
	ProcEnemy::fixedUpdate(dt);
}

void ArabBurserker::renderObj(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(collider());
	drawRect(attkCollider());

#endif // DEBUG
	setRGBA(1, 1, 1, 1);
}

void ArabBurserker::releaseObj()
{
	for (int i = 0; i < 6; i++)
		delete imgEnemy[i];
	delete imgEnemy;
	imgEnemy = NULL;
}



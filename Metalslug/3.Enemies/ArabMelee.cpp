#include "ArabMelee.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgMeleeInfo[];
static iImage** _imgMelee = NULL;
ArabMelee::ArabMelee(int index) : ProcEnemy(index)
{
	collider = new Collider();
	collider->init(this);

	index = IdxArMelee;
	state = IdleEnemyL;	
	ai = AI::enemyAI0;

	hp = 100;
	dmg = 100;
	sight = 200;
	moveSpeed = 100;
	attkRange = 70;
	attkRate = 0.f;	_attkRate = 2.f;
	aiDt = 0.f;	_aiDt = 2.f;
	///////////////////////////////////
	imgs = NULL;
	imgCurr = NULL;	

	up = 0.f;
	down = 0.f;
	fall = false;

	if( _imgMelee==NULL )
		_imgMelee = createImgReverse(imgMeleeInfo, EnemyBehaveMax, this);

	imgs = new iImage * [EnemyBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * EnemyBehaveMax);
	for (int i = 0; i < EnemyBehaveMax; i++)
		imgs[i] = _imgMelee[i]->clone();
}

ArabMelee::~ArabMelee()
{	
	for (int i = 0; i < EnemyBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

void ArabMelee::dead()
{
	state = (EnemyBehave)(DeadEnemyL + state % 2);
	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
}

void ArabMelee::setState(EnemyBehave newState)
{
	state = newState;

	if (state == AttackEnemyL || state == AttackEnemyR)
	{
		//imgs[newState]->startAnimation(ProcEnemy::cbAniAttack, this);
	}
	if (state == DeadEnemyL || state == DeadEnemyR)
	{
		//imgs[newState]->startAnimation(ProcEnemy::cbAniDead, this);
	}
}

void ArabMelee::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-bg->off.x - 20, -bg->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		ai(this, dt);
	}
	p.y = *(bg->maxY + (int)p.x);
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

bool ArabMelee::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(collider->getCollider());
#endif // DEBUG
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabMelee::free()
{
	//#issue한번만 지우기
	if (_imgMelee != NULL)
	{
		for (int i = 0; i < EnemyBehaveMax; i++)
			delete _imgMelee[i];
		delete _imgMelee;
		_imgMelee = NULL;
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
		AnimationMgr::cbAniDead,		
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
		"assets/ArabMelee/ArabMelee_AttackMelee_%02d.png",
		8, 1.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_AttackPre_%02d.png",
		4, 1.0f, { -36, 0},
		0.18f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
};

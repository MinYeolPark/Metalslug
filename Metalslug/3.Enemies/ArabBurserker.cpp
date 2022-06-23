#include "ArabBurserker.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgBurserkInfo[];
static iImage** _imgBurserk = NULL;
ArabBurserker::ArabBurserker(int index) : ProcEnemy(index)
{	
	collider->init(this, iSizeMake(50, 50));

	index = IdxArBurserker;
	state = IdleEnemyL;
	ai = ProcEnemyAI::ArabBurserkAI0;

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

	if (_imgBurserk == NULL)
		_imgBurserk = createImgReverse(imgBurserkInfo, EnemyBehaveMax, this);

	imgs = new iImage * [EnemyBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * EnemyBehaveMax);
	for (int i = 0; i < EnemyBehaveMax; i++)
		imgs[i] = _imgBurserk[i]->clone();
	imgCurr = imgs[index];
}

ArabBurserker::~ArabBurserker()
{
	for (int i = 0; i < EnemyBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

static iPoint initPos;
bool ArabBurserker::dead()
{
	collider->disable();

	initPos = p;
	state = (EnemyBehave)(DeadEnemyL + state % 2);
	if (state == DeadEnemyL)
		v.x = 1;
	else//if(state==DeadEnemyR)
		v.x = -1;

	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
	return state == (EnemyBehave)(DeadEnemyL + state % 2);
}

void ArabBurserker::setState(EnemyBehave newState)
{
	state = newState;

	if (state == AttackEnemyL || state == AttackEnemyR)
	{
		//imgs[newState]->startAnimation(ProcEnemy::cbAniAttack, this);
	}
}

void ArabBurserker::update(float dt)
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

	if (tp != iPointMake(-1, -1))
	{
		//if (movePoint(p, p, tp, moveSpeed * dt))
		//{
		//	
		//}
	}
	p.y = *(bg->maxY + (int)p.x);
	printf("%d\n", (int)state);
	if (getState() == DeadEnemyL)
	{
		v.x = 1;
		movePoint(p, p, { initPos.x + 50, 0 }, moveSpeed);
		printf("Dead");
	}
	else if (getState() == DeadEnemyR)
	{
		v.x = -1;
		movePoint(p, p, { initPos.x - 50, 0 }, moveSpeed);
		printf("Dead");

	}
	p += v * moveSpeed * dt;
	fixedUpdate(dt);
}
bool ArabBurserker::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(collider->getCollider());
#endif
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabBurserker::free()
{
	//#issue한번만 지우기
	if (_imgBurserk != NULL)
	{
		for (int i = 0; i < EnemyBehaveMax; i++)
			delete _imgBurserk[i];
		delete _imgBurserk;
		_imgBurserk = NULL;
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
		AnimationMgr::cbAniDead,
	},
};

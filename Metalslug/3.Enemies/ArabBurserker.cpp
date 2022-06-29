#include "ArabBurserker.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgBurserkInfo[];
static iImage** _imgBurserk = NULL;
ArabBurserker::ArabBurserker(int index) : ProcEnemy(index)
{		
	index = IdxArBurserker;
	state = IdleBurserkL;
	ai = ProcEnemyAI::ArabBurserkAI0;

	hp = 100;
	dmg = 100;
	sight = 200;
	moveSpeed = 100;
	attkRange = 70;
	attkRate = 0.f;	_attkRate = 2.f;
	aiDt = 0.f;	_aiDt = 2.f;
	///////////////////////////////////
#if 1
	colNum = 1;
	for (int i = 0; i < colNum; i++)
		colliders[i]->init(this, iSizeMake(40, 40));
#endif
	imgs = NULL;
	imgCurr = NULL;

	up = 0.f;
	down = 0.f;
	fall = false;

	if (_imgBurserk == NULL)
		_imgBurserk = createImgReverse(imgBurserkInfo, BurserkBehaveMax, this);

	imgs = new iImage * [BurserkBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * BurserkBehaveMax);
	for (int i = 0; i < BurserkBehaveMax; i++)
		imgs[i] = _imgBurserk[i]->clone();
	imgCurr = imgs[index];
}

ArabBurserker::~ArabBurserker()
{
	for (int i = 0; i < BurserkBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

bool ArabBurserker::dead()
{
	isDead = true;
	for (int i = 0; i < colNum; i++)
	{
		colliders[i]->disable();
		//objects->removeObject(colliders[i]);
	}
	state = (DeadBurserkL + state % 2);
	if (state == DeadBurserkL)
		v.x = 1;
	else//if(state==DeadEnemyR)
		v.x = -1;

	

	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
	return state == (DeadBurserkL + state % 2);
}

void ArabBurserker::getDamage(float damage, Collider* c)
{
	hp -= damage;
	if (hp <= 0)
		dead();
}

void ArabBurserker::setState(int newState)
{
	state = newState;
}

void ArabBurserker::update(float dt)
{	
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		ai(this, dt);
	}

	if (v != iPointZero)
	{
		if (v.x > 0)
			setState(WalkBurserkR);
		else if (v.x > 0)
			setState(WalkBurserkL);
	}
	p.y = *(map->maxY + (int)p.x);
	p += v;
	if (-map->off.x + 40 > p.x)
	{
		v.x = 1;
	}
	for (int i = 0; i < colNum; i++)
		colliders[i]->setPosition(p);

	fixedUpdate(dt);
}
bool ArabBurserker::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(colliders[0]->getCollider());
	for (int i = 0; i < 1; i++)
	{
		drawRect(colliders[i]->getCollider());
	}
#endif
	if (isDead)
		movePoint(p, p, p += (v), dt);
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabBurserker::free()
{
	//#issue한번만 지우기
	if (_imgBurserk != NULL)
	{
		for (int i = 0; i < BurserkBehaveMax; i++)
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

#include "ArabBurserker.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgBurserkInfo[];
static iImage** _imgBurserk = NULL;
ArabBurserker::ArabBurserker(int index) : ProcEnemy(index)
{		
	this->index = index;
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

	rectNum = 1;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();
}

ArabBurserker::~ArabBurserker()
{
	for (int i = 0; i < BurserkBehaveMax; i++)
		delete imgs[i];
	delete imgs;

	for (int i = 0; i < rectNum; i++)
		delete rect[i];
	delete rect;
}


void ArabBurserker::init(iPoint p)
{
	this->isActive = true;
	this->index = index;
	this->p = p;
	this->tp = p;
	state = IdleBurserkL;

	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(40, 40);
		r->origin = p;
	}
}

void ArabBurserker::update(float dt)
{	
	if (isDead)
	{
		for (int i = 0; i < rectNum; i++)
			rect[i]->size = iSizeZero;
		return;
	}

	if (!isAppear)
	{
		if (containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80)))
			isAppear = true;
	}
	else
	{
		isActive = containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80));
	}

	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		ai(this, dt);
	}
	if (p != tp)
	{
		if (p.x < tp.x)
		{
			p.x += v.x;
			if (p.x > tp.x)
				p.x = tp.x;
		}
		else if (p.x > tp.x)
		{
			p.x += v.x;
			if (p.x < tp.x)
				p.x = tp.x;
		}

		if (p.y < tp.y)
		{
			p.y += v.y;
			if (p.y > tp.y)
				p.y = tp.y;
		}
		else if (p.y > tp.y)
		{
			p.y += v.y;
			if (p.y < tp.y)
				p.y = tp.y;
		}

		if (p == tp)
			v = iPointZero;
	}

	if (v != iPointZero)
	{
		if (v.x > 0)
			setState(WalkBurserkR);
		else if (v.x < 0)
			setState(WalkBurserkL);

		//for (int i = 0; i < player->colNum; i++)
		//{
		//	Collider* c = player->colliders[i];
		//	if (containPoint(p, c->getCollider()))
		//	{
		//		float d = iPointLength(p - c->p);

		//		if (!player->isDead)
		//		{
		//			player->getDamage(100, c);
		//			iPoint bp = iPointMake(rand() % 10 + p.x, rand() % 10 + p.y);
		//			addProcEffect(EffectBlood, bp);		//bulletIndex=effectIndex
		//		}
		//	}			
		//}		
	}
	else
	{
		//attack pattern		
		//Except
		if (player->isDead)
		{
			if (state != (ArabBurserkBehave)IdleBurserkL + state % 2)
				setState((ArabBurserkBehave)IdleBurserkL + state % 2);
		}
	}
	fixedUpdate(dt);
}
void ArabBurserker::fixedUpdate(float dt)
{
	ProcEnemy::fixedUpdate(dt);

	//ColliderUpdate
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			p.x + map->off.x - rect[i]->size.width / 2,
			p.y + map->off.y - rect[i]->size.height);
	}
}
bool ArabBurserker::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

	if (alphaDt)
	{
		alphaDt += dt;
		alpha = fabsf(_cos((alphaDt / _alphaDt * 270)));
		if (alphaDt > _alphaDt)
		{
			alphaDt = 0.0f;
			isActive = false;
		}
	}
#ifdef _DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect());

#endif // _DEBUG

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

void ArabBurserker::getDamage(float damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		if (!isDead)
		{
			isDead = true;
			state = (DeadBurserkL + state % 2);
			imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
		}
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

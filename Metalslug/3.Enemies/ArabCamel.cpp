#include "ArabCamel.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgArabCamelInfo[];
ImageInfo imgCamelInfo[];
static iImage** _imgArabCamel = NULL;
static iImage** _imgCamel = NULL;
ArabCamel::ArabCamel(int index) : ProcEnemy(index)
{
	index = IdxArCamel;
	state = 0;
	camelState = CamelRunL;
	arabState = ArabCamelFireL;
	ai = (EnemyAI)ProcEnemyAI::ArabCamelAI0;

	hp = 1000;
	dmg = 100;
	sight = 200;
	moveSpeed = 100.f;
	attkRange = 70;
	attkRate = 0.f;	_attkRate = 2.f;
	aiDt = 0.f;	_aiDt = 2.f;
	///////////////////////////////////

	camelImgs = NULL;
	camelImgCurr = NULL;
	arabImgs = NULL;
	arabImgCurr = NULL;

	arabPos = p;
	up = 0.f;
	down = 0.f;
	fall = false;

	if (_imgCamel == NULL)
		_imgCamel = createImgReverse(imgCamelInfo, CamelBehaveMax, this);
	if (_imgArabCamel == NULL)
		_imgArabCamel = createImgReverse(imgArabCamelInfo, ArabCamelBehaveMax, this);

	camelImgs = new iImage * [CamelBehaveMax];
	memset(camelImgs, 0x00, sizeof(iImage*) * CamelBehaveMax);
	for (int i = 0; i < CamelBehaveMax; i++)
		camelImgs[i] = _imgCamel[i]->clone();
	arabImgs = new iImage * [ArabCamelBehaveMax];
	memset(arabImgs, 0x00, sizeof(iImage*) * ArabCamelBehaveMax);
	for (int i = 0; i < ArabCamelBehaveMax; i++)
		arabImgs[i] = _imgArabCamel[i]->clone();	
}

ArabCamel::~ArabCamel()
{
	for (int i = 0; i < ArabCamelBehaveMax; i++)
	{
		delete camelImgs[i];
		delete arabImgs[i];
	}
	delete camelImgs;
	delete arabImgs;
}

void ArabCamel::getDamage(float damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		
	}
}

void ArabCamel::init(iPoint p)
{
}

void ArabCamel::update(float dt)
{
	p.y = *(map->maxY + (int)p.x);
	if (arabState != (ArabCamelBehave)(ArabCamelDeadL + arabState % 2))
		arabPos = { p.x, p.y - 42 };
	else//
	{				
		movePoint(arabPos, arabPos, { arabPos.x,p.y }, 0.5);
	}
	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		//ai(this, dt);
	}

	if (camelState == CamelDeadL)
		v.x -= 1;
	else if (camelState == CamelDeadR)
		v.x = 1;

	if (camelState == CamelDeadL || camelState == CamelDeadR)
		p += v * (moveSpeed / 2 * dt);
	
	p += iPointMake(-1,0) * moveSpeed * dt;
	fixedUpdate(dt);
}

void ArabCamel::fixedUpdate(float dt)
{	
	int maxY = *(map->maxY + (int)p.x);
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

bool ArabCamel::draw(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, 1);
	arabImgCurr = arabImgs[arabState];
	arabImgCurr->paint(dt, arabPos + off);
	camelImgCurr = camelImgs[camelState];
	camelImgCurr->paint(dt, p + off);

	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabCamel::free()
{
	//#issue한번만 지우기
	if (_imgArabCamel != NULL)
	{
		for (int i = 0; i < ArabCamelBehaveMax; i++)
			delete _imgArabCamel[i];
		delete _imgArabCamel;
		_imgArabCamel = NULL;
	}
	if (_imgCamel != NULL)
	{
		for (int i = 0; i < CamelBehaveMax; i++)
			delete _imgCamel[i];
		delete _imgCamel;
		_imgCamel = NULL;
	}
}



ImageInfo imgArabCamelInfo[] =
{
	{
		"assets/ArabCamel/ArabCamel_Idle_%02d.png",
		3, 1.0f, { -6 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/ArabCamel_Appear_%02d.png",
		10, 1.0f, { -6 / 2, 0},
		0.1f,
		1,			//repeat
		{255, 255, 255, 255},
		AnimationMgr::cbAniEnemyMotion2Idle,
	},
	{
		"assets/ArabCamel/ArabCamel_Fire_%02d.png",
		10, 1.0f, { -72 / 2, 0},
		0.1f,
		3,			//repeat
		{255, 255, 255, 255},
		AnimationMgr::cbAniEnemyMotion2Idle,
	},
	{
		"assets/ArabCamel/ArabCamel_Down_%02d.png",
		3, 1.0f, { -51 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/ArabCamel_DownIdle_%02d.png",
		3, 1.0f, { -38 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/ArabCamel_DownFire_%02d.png",
		10, 1.0f, { -51 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/ArabCamel_Dead_%02d.png",
		10, 1.0f, { -45 / 2, 0},
		0.18f,
		1,			//repeat
		{255, 255, 255, 255},
		AnimationMgr::cbAniDead,
	},
};
ImageInfo imgCamelInfo[] =
{
	{
		"assets/ArabCamel/Camel_Run_%02d.png",
		12, 1.0f, { -64 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/Camel_Down_%02d.png",
		3, 1.0f, { -64 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/Camel_Idle_%02d.png",
		3, 1.0f, { -64 / 2, 0},
		0.12f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/ArabCamel/Camel_Dead_%02d.png",
		12, 1.0f, { -64 / 2, 0},
		0.06f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
};
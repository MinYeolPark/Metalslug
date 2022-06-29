#include "Abul.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgAbulInfo[];
static iImage** _imgAbul = NULL;
Abul::Abul(int index) : ProcEnemy(index)
{
	this->index = index;
	state = IdleAbulL;

	hp = 0;
	dmg = 0;
	sight = 0;
	moveSpeed = 100.f;
	attkRange = 0;
	attkRate = 0.f;	_attkRate = 0.f;
	aiDt = 0.f;	_aiDt = 0.f;

	colNum = 0;

    imgs = NULL;
    imgCurr = NULL;

	up = 0.f;
	down = 0.f;
	fall = false;

	if (_imgAbul == NULL)
		_imgAbul = createImgReverse(imgAbulInfo, AbulBehaveMax, this);

	imgs = new iImage * [AbulBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * AbulBehaveMax);
	for (int i = 0; i < AbulBehaveMax; i++)
		imgs[i] = _imgAbul[i]->clone();
	imgCurr = imgs[index];
}

Abul::~Abul()
{
}

int Abul::getFrame()
{
    return 0;
}

bool Abul::dead()
{
	isDead = true;
	state = (SurrenderAbulL + state % 2);

    return false;
}

void Abul::getDamage(float damage, Collider* c)
{
	
}

void Abul::setState(int newState)
{
	state = newState;
}

void Abul::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 40, -map->off.y - 40,
			devSize.width + 80, devSize.height + 80));

	if (!isAppear)
	{

	}
	else
	{

	}

	fixedUpdate(dt);
}

bool Abul::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);
#ifdef _DEBUG
	drawDot(p + off);
	for (int i = 0; i < colNum; i++)
		drawRect(colliders[i]->getCollider());
	setRGBA(1, 1, 1, 0.5);
#endif // DEBUG
	setRGBA(1, 1, 1, 1);

    return !isActive;
}

void Abul::free()
{
}



ImageInfo imgAbulInfo[] =
{
	{
		"assets/Abul/Abul_Idle_%02d.png",
		4, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	},
	{
		"assets/Abul/Abul_Signal_%02d.png",
		9, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	},
	{
		"assets/Abul/Abul_Surrender_%02d.png",
		17, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	}
};
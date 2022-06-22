#include "Ichimondi.h"

#include "ImgMgr.h"
#include "ProcItem.h"
static iImage** _imgIchi = NULL;
ImageInfo infoIchi[];
Ichimondi::Ichimondi(int idx): ProcNpc()
{		
	imgs = NULL;
	imgCurr = NULL;
	state = (NpcBehave)0;
	if(_imgIchi == NULL)
		_imgIchi = createImgReverse(infoIchi, NpcBehaveMax, this);

	imgs = new iImage * [NpcBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * NpcBehaveMax);
	for (int i = 0; i < NpcBehaveMax; i++)
		imgs[i] = _imgIchi[i]->clone();
	imgCurr = imgs[index];
}

Ichimondi::~Ichimondi()
{
	for (int i = 0; i < NpcIndexMax; i++)
		delete _imgIchi[i];
	delete _imgIchi;

	_imgIchi = NULL;
}

iRect Ichimondi::collider()
{
	return iRectMake(p.x + bg->off.x - 20, p.y + bg->off.y - 40, 40, 40);
}

void Ichimondi::spawnItem()
{
	addProcItem(itemIndex, this->p - bg->off);
}

void Ichimondi::setState(NpcBehave newState)
{
	state = newState;
}

void Ichimondi::set(int index, iPoint p)
{
	isActive = true;
	int r = rand() % ItemIndexMax;
	itemIndex = r;
	this->p = p;
	this->index = (NpcIndex)index;
}

void Ichimondi::set(int idex, iPoint p, int itemIndex)
{
	isActive = true;
	this->itemIndex = itemIndex;
	this->p = p;
	this->index = (NpcIndex)index;
}

void Ichimondi::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-bg->off.x - 20, -bg->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	if (!complete)
	{
		if (state == WalkR || state == WalkL)
		{
			if (containPoint(player->p, collider()))		
				setState((NpcBehave)(AddItemNpcL + state % 2));
		}
	}	

	if (v != iPointZero)
	{
		if (v.x > 0)
			setState(EscapeNpcR);
		else if (v.x < 0)
			setState(EscapeNpcL);
		p += v * moveSpeed * dt;
	}

	fixedUpdate(dt);
}

void Ichimondi::fixedUpdate(float dt)
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
}

#include "InputMgr.h"
bool Ichimondi::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);	
	setRGBA(1, 1, 1, 1);

#ifdef _DEBUG
	drawDot(p + off);
	drawRect(collider());
#endif // _DEBUG

	return !isActive;
}


ImageInfo infoIchi[] =
{
	{
		"assets/NPC/Bondage_Sit_%02d.png",
		4, 1.0f, { -91 / 2, 0},
		0.18f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/NPC/Bondage_Release_%02d.png",
		4, 1.0f, { -91 / 2, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/NPC/Walk_%02d.png",
		12, 1.0f, { -91 / 2, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/NPC/AddItem_%02d.png",
		10, 1.0f, { -91 / 2, 0},
		0.12f,
		1,
		{255, 0, 0, 255},
		ProcNpc::cbAniSpawnItem,
	},
	{
		"assets/NPC/Salute_%02d.png",
		14, 1.0f, { -91 / 2, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		ProcNpc::cbAniSalute,
	},
	{
		"assets/NPC/Escape_%02d.png",
		8, 1.0f, { -91 / 2, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
};

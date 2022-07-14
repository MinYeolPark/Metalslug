#include "Ichimondi.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"

#include "ProcItem.h"
static iImage** _imgIchi = NULL;
ImageInfo infoIchi[];
Ichimondi::Ichimondi(int index): ProcNpc(index)
{		
	//colliders[0]->init(this, iSizeMake(50, 50));

	index = NpcIchimondi;
	state = IdleNpcL;
	ai = NULL;
	complete = false;
	escape = false;
	itemIndex = -1;
	///////////////////////////////////
	imgs = NULL;
	imgCurr = NULL;

	moveSpeed = 50.f;
	up = 0.f;
	down = 0.f;
	fall = false;

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

void Ichimondi::dead()
{
	state = (NpcBehave)(WalkNpcL + state % 2);
	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
}

void Ichimondi::spawnItem()
{
	addProcItem(itemIndex, this->p - map->off);
}
iPoint initPos = iPointZero;
void Ichimondi::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	if (v != iPointZero)
	{
		if (getState() == WalkNpcL || getState() == WalkNpcR)
		{
			if(initPos==iPointZero)
				initPos= p;
			int maxX;
			if (v.x > 0)
			{
				maxX = initPos.x + 100;
				setState(WalkNpcR);
				if (p.x > maxX)
					v.x = -1;
			}
			else if (v.x < 0)
			{
				maxX = initPos.y - 100;
				setState(WalkNpcL);			
				if (p.x < maxX)
					v.x = 1;
			}
		}
			
	}
	if (getState() == WalkNpcL || getState() == WalkNpcR)
	{
		if (containPoint(player->p, iRectMake(p.x - 20, p.y - 40, 40, 40)))
		{
			v = iPointZero;
			setState((NpcBehave)(AddItemNpcL + state % 2));
		}
	}

	if (getState() == EscapeNpcL || getState() == EscapeNpcR)
		p += v * moveSpeed * 4 * dt;
	else
		p += v * moveSpeed * dt;		

	fixedUpdate(dt);
}

bool Ichimondi::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);	
	setRGBA(1, 1, 1, 1);

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
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniNpcRelease,
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
		AnimationMgr::cbAniNpcSpawnItem,
	},
	{
		"assets/NPC/Salute_%02d.png",
		14, 1.0f, { -91 / 2, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniNpcSalute,
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

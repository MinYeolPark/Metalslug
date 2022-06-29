#include "ProcItem.h"

#include "ImgMgr.h"

ProcItem** _item;
ProcItem** item;
int itemNum;
void loadProcItem()
{
	_item = new ProcItem * [ItemIndexMax];
	for (int i = 0; i < ItemIndexMax; i++)
		_item[i] = new ProcItem[itemMax];
	item = new ProcItem * [ItemIndexMax * itemMax];
	itemNum = 0;
}
void freeProcItem()
{
	for (int i = 0; i < ItemIndexMax; i++)
	{
		delete _item[i];
		delete item[i];
	}
	delete _item;
	delete item;
}
void drawProcItem(float dt, iPoint off)
{
	for (int i = 0; i < itemNum; i++)
	{
		ProcItem* m = item[i];
		m->update(dt);
		if (m->draw(dt, off))
		{
			itemNum--;
			item[i] = item[itemNum];
			i--;
		}
	}
}
void addProcItem(int index, iPoint p)
{
	for (int i = 0; i < itemMax; i++)
	{
		ProcItem* m = &_item[index][i];
		if (m->isActive == false)
		{
			m->init(index, p);
			item[itemNum] = m;
			itemNum++;
			return;
		}
	}
}
static iImage** _imgItem = NULL;
ImageInfo infoItem[];
ProcItem::ProcItem()
{
	imgs = NULL;
	imgCurr = NULL;
	p = iPointZero;
	s = iSizeZero;

	index = (ItemIndex)0;
	isActive = false;

	_imgItem = createSingleImage(infoItem, ItemIndexMax, this);
	imgs = new iImage * [ItemIndexMax];
	memset(imgs, 0x00, sizeof(iImage*) * ItemIndexMax);
	for (int i = 0; i < ItemIndexMax; i++)
		imgs[i] = _imgItem[i]->clone();
	imgCurr = imgs[index];
}

ProcItem::~ProcItem()
{
}

void ProcItem::init(int index, iPoint p)
{
	isActive = true;
	this->index = (ItemIndex)index;
	this->p = p;
}

void ProcItem::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));
	if (containPoint(player->p, collider()))
	{
		if (index == HeavyMachinegun)
		{
			player->changeGun(HeavyMachinegun);
		}
		else if (index == Bomb)
		{
			player->addBomb(20);
		}
		isActive = false;
	}
}

bool ProcItem::draw(float dt, iPoint off)
{
	imgCurr = imgs[index];
	imgCurr->paint(dt, p + off);


#ifdef _DEBUG
	setRGBA(1, 0, 1, 0.5);
	drawRect(collider());
	setRGBA(1, 1, 1, 1);
#endif // _DEBUG	
	return !isActive;
}

void ProcItem::free()
{
}

iRect ProcItem::collider()
{
	return iRectMake(	p.x + map->off.x,
						p.y + map->off.y - imgCurr->tex->height,
						imgCurr->tex->width,
						imgCurr->tex->height);
}

ImageInfo infoItem[] =
{
	{
		"assets/Item/Bomb_%02d.png",
		6, 1.0f, { 0, 0},
		0.08f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/Item/Machinegun_%02d.png",
		4, 1.0f, { 0, 0},
		0.08f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
	{
		"assets/Item/Bomb_%02d.png",
		6, 1.0f, { 0, 0},
		0.08f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
};
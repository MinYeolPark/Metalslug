#include "ProcItem.h"

#include "ImgMgr.h"

ProcItem*** _item;
ProcItem** item;
int itemNum;
void loadProcItem()
{
	_item = new ProcItem * * [ItemIndexMax];
	for (int i = 0; i < ItemIndexMax; i++)
	{
		_item[i] = new ProcItem * [itemMax];
		for (int j = 0; j < itemMax; j++)
			_item[i][j] = new ProcItem(i);
	}
	item = new ProcItem * [ItemIndexMax * itemMax];
	itemNum = 0;
}
void freeProcItem()
{
	for (int i = 0; i < ItemIndexMax; i++)
	{
		for (int j = 0; j < itemNum; j++)
		{
			_item[i][j]->free();
			delete _item[i][j];			
		}
		delete _item[i];
	}
	delete _item;
	delete item;
}
void drawProcItem(float dt, iPoint off)
{
	for (int i = 0; i < itemNum; i++)
	{
		ProcItem* m = item[i];
		if (m->isActive)
		{
			m->update(dt);
			if (m->draw(dt, off))
			{
				itemNum--;
				item[i] = item[itemNum];
				i--;
			}
		}		
	}
}
void addProcItem(int index, iPoint p)
{
	for (int i = 0; i < itemMax; i++)
	{
		ProcItem* m = _item[index][i];
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
ProcItem::ProcItem(int index) : ProcObject()
{
	imgs = NULL;
	imgCurr = NULL;
	p = iPointZero;
	s = iSizeZero;

	index = 0;
	isActive = false;

	if (_imgItem == NULL)
		_imgItem = createSingleImage(infoItem, ItemIndexMax, this);

	imgs = new iImage * [ItemIndexMax];
	memset(imgs, 0x00, sizeof(iImage*) * ItemIndexMax);
	for (int i = 0; i < ItemIndexMax; i++)
		imgs[i] = _imgItem[i]->clone();
	imgCurr = imgs[index];

	rectNum = 1;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();
}

ProcItem::~ProcItem()
{
}

void ProcItem::init(int index, iPoint p)
{
	isActive = true;
	this->index = index;
	this->p = p;	

	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(30, 30);
		r->origin = p;
	}
}

void ProcItem::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 20, -map->off.y - 20,
			devSize.width + 40, devSize.height + 40));

	//ColliderUpdate
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			p.x + map->off.x - rect[i]->size.width / 2,
			p.y + map->off.y - rect[i]->size.height);
		if (containPoint(player->p, *rect[i]))
		{
			if (index == ItemHeavyMachineGun)
			{
				player->changeGun(HeavyMachinegun);
			}
			else if (index == ItemBomb)
			{
				player->addBomb(10);
			}
			isActive = false;
		}
	}
}

bool ProcItem::draw(float dt, iPoint off)
{
	imgCurr = imgs[index];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect());
#endif // _DEBUG	
	return !isActive;
}

void ProcItem::free()
{
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
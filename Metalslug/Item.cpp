#include "Item.h"

#include "ImgMgr.h"

ImageInfo itemImage[];
Item* item;
Item::Item()
{
	imgs = createImgBullets(itemImage, this);	

	itemIndex = ItemIndex::ItemDefault;
	p = iPointZero;
	s = iSizeZero;
}

Item::~Item()
{
}

void Item::addItem(iPoint p, int idx)
{
}

void Item::init()
{
}

void Item::update()
{

}

void Item::draw(float dt, iPoint off)
{
	imgCurItem = imgs[itemIndex];
	imgCurItem->paint(dt, off);
}

void Item::free()
{
}

iRect Item::collider()
{
	return iRect();
}

ImageInfo itemImage[] =
{
	{
		"assets/Item/HeavyMachinegun_%02d.png",
		4, 1.0f, { 0, 0},
		0.18f,
		0,			//repeat
		{255, 255, 255, 255},
		NULL,
	},
};
#pragma once

#include "iStd.h"

enum ItemIndex
{
	ItemDefault,
	ItemGun,
	ItemBomb,
	ItemScore,

	ItemIndexMax,
};

class Item
{
	Item();
	virtual ~Item();

public:
	iImage** imgs;
	iImage* imgCurItem;
	iPoint p;
	iSize s;

	ItemIndex itemIndex;


public:
	void addItem(iPoint p, int idx);
public:
	void init();
	void update();
	void draw(float dt, iPoint off);
	void free();
public:
	iRect collider();
};

extern Item* item;
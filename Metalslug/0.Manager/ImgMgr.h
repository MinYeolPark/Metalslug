#pragma once

#include "iStd.h"
#include "Proc.h"

struct ImageInfo
{
	const char* path;
	int num;
	float s;
	iPoint p;

	float aniDt;
	int repeatNum;
	iColor4b colorKey;
	cbAnimation cbAni;
};

iImage** createImgSelect(Texture* map);

iImage** createImgChar(ImageInfo* imageInfo, void* c);
iImage** createImgPlayer(ImageInfo* imageInfo, int size, void* c);

iImage** createImgBullets(ImageInfo* imageInfo, void* b);
iImage** createImgEffect(ImageInfo* imageInfo, void* c);

iImage** createImgEnemy(ImageInfo* imageInfo, void* c);
iImage** createImgReverse(ImageInfo* imageInfo, int size, void* c);

iImage** createImgSingleBehave(ImageInfo* imageInfo, void* c);
iImage** createImgSingleEach(ImageInfo* imageInfo, void* c);



iImage** createSingleImage(ImageInfo* imageInfo, int size, void* c);


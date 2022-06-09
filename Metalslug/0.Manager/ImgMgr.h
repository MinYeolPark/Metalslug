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
	int anc;
	cbAnimation cbAni;
};

iImage** createImgChar(ImageInfo* imageInfo, void* c);
iImage** createImgPlayer(ImageInfo* imageInfo, void* c);

iImage** createImgBullets(ImageInfo* imageInfo);
iImage** createImgEffect(ImageInfo* imageInfo, void* c);

iImage** createImgEnemy(ImageInfo* imageInfo, void* c);
iImage** createImgEnemyReverse(ImageInfo* imageInfo, int size, void* c);

iImage** createImgSingleBehave(ImageInfo* imageInfo, void* c);
iImage** createImgSingleEach(ImageInfo* imageInfo, void* c);



iImage** createMosqueBaseImage(ImageInfo* imageInfo, int size, void* c);


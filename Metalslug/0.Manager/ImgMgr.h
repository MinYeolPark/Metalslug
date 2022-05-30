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
	iColor4f colorKey;
	cbAnimation cbAni;
};

iImage** createImgChar(ImageInfo* imageInfo, void* c);
iImage** createImgCharReverse(ImageInfo* imageInfo, void* c);

iImage** createImgBullets(ImageInfo* imageInfo, void* c);

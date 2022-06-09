#pragma once

#include "iStd.h"

struct BgData
{
	const char* rscName;
	iPoint p[20];
	int pCount;
	float scale;
};

struct BgLayer
{
	const char* rscName;
	float scale;	
	iPoint offSet;
	float rate;
};

struct Bg
{
	Bg();
	virtual ~Bg();

	void paint(float dt);
	iPoint move(iPoint mp);

	BgData* bd;
	BgLayer* bl;

	Texture** texs;			//main map Layer
	Texture** bgTexs;		//background Layer
	iPoint off;				//좌 상단 anchor
	iPoint offMin;			//움직일 수 있는 최소 범위
	iPoint offMax;

	int maxW;			//Bg texture 넓이
	int* maxY;			//Bg texture 높이
};
#define cuts_Stage1 12
#define layers_Stage1 3

extern BgData bgData_Stage1[cuts_Stage1];
extern BgLayer bgLayer_Stage1[layers_Stage1];
extern Bg* bg;

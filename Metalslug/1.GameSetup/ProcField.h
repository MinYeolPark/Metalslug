#pragma once
#include "iStd.h"

struct MapData
{
	iPoint p;
	iPoint point[20];
	int pCount;
};
struct LayerData
{
	iPoint offSet;
	float rate;		//move rate
};
class ProcMap
{
public:
	ProcMap(int stage);
	virtual ~ProcMap();

public:
	int stage;
	int maxW;			//map texture 넓이
	int* maxY;			//map texture 높이
	bool viewChange;
	bool isClipped;
	iImage** imgs;
	iImage** imgsLayer;
	iImage* imgObj[3];	//0: Step, 2: bossLayer

	MapData* md;
	LayerData* ld;

	iPoint off;				//좌 상단 anchor
	iPoint offMin;			//움직일 수 있는 최소 범위
	iPoint offMax;

public:
	void init(int stage);
	void update(float dt);
	void paint(float dt);
	bool move(iPoint mp);

	bool clip(iPoint p);
};
#define layerNum 3
extern ProcMap* map;

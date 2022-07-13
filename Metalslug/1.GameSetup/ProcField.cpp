#include "ProcField.h"

#include "ProcPlayer.h"
#include "ImgMgr.h"

#include "InputMgr.h"
int mapNum[] =
{
	6,
};
LayerData layerData[layerNum] =
{
	{
		{0,90},
		0.2f,
	},
	{
		{0,105},
		0.5f,
	},
	{
		{0,160},
		0.8f,
	},
};
ProcMap* map;
MapData mapData[];
ImageInfo mapImageInfo[];
ImageInfo layerImageInfo[];
ProcMap::ProcMap(int stage)
{
	//Map
	this->stage = stage;
	maxW = 0;
	maxY = 0;
	viewChange = false;
	isClipped = false;

	imgs = new iImage * [mapNum[stage]];
	imgs = createSingleImage(mapImageInfo,	mapNum[stage], this);	
	//	
	//Layer	
	imgsLayer = new iImage * [layerNum];
	imgsLayer = createSingleImage(layerImageInfo, layerNum, this);	
}

ProcMap::~ProcMap()
{
	/*for (int i = 0; i < mapNum[stage]; i++)
		delete imgs;
	delete imgs;*/
}

void ProcMap::init(int stage)
{
	for (int i = 0; i < mapNum[stage]; i++)
	{
		Texture* t = imgs[i]->tex;
		maxW += t->width;
	}	

	maxW = 1176 + 786 + 550 + 1008 + 331;
	printf("maxW = %d\n", maxW);
	maxY = new int[maxW];	
	for (int j = 0; j < mapNum[stage]; j++)
	{
		md = &mapData[j];
		for (int k = 0; k < md->pCount; k++)
		{
			iPoint sp = md->point[k];
			iPoint ep = md->point[1 + k];

			for (int m = sp.x; m < ep.x; m++)
			{
				float a = (m - sp.x) / (ep.x - sp.x);
				maxY[m] = sp.y + (ep.y - sp.y) * a;
			}
		}
	}
	off = iPointZero;
	offMax = iPointMake(0, 18);
	offMin = iPointMake(devSize.width - maxW, 0);
}
void ProcMap::update(float dt)
{
	//Camera Move
	float x = player->p.x + off.x;
	
	if (!viewChange)
	{
		if (player->p.x > 1050)
		{
			if (move(iPointMake(100 * dt, 100 * dt)))
			{
				viewChange = true;			
				//bg collider -> true : block to backward
			}
		}
	}
	if (player->p.x > 3600)
	{		
		if (!isClipped)
		{
			if (move(iPointMake(-100, 0)))
			{
				isClipped = true;
				printf("clipped\n");
			}
			
		}
		//if (move(iPointMake(devSize.width / 3 - tx, 0)))
		//{
		//	isClipped = true;
		//	printf("clipped\n");
		//}
	}

	if (!isClipped)
	{		
		if (off.x > offMax.x || off.x < offMin.x)
			return;

		if (x < devSize.width / 3)
			move(iPointMake(devSize.width / 3 - x, 0));
		else if (x > devSize.width * 2 / 3)
			move(iPointMake(devSize.width * 2 / 3 - x, 0));
	}
	else
		return;
}
#include "InputMgr.h"
void ProcMap::paint(float dt)
{
	iPoint p;
	//Layer
	for (int i = 0; i < layerNum; i++)
	{
		ld = &layerData[i];
		Texture* t = imgsLayer[i]->tex;
		p.x = ld->offSet.x + off.x * ld->rate;
		p.y = ld->offSet.y;
		while (p.x < -t->width * layerImageInfo[i].s)
			p.x += t->width * layerImageInfo[i].s;
		for (int j = 0; j < 3; j++)
		{
			imgsLayer[i]->paint(dt, 
				iPointMake(p.x + t->width * j * layerImageInfo[i].s, p.y));
		}
	}
	//Map
	//float ay[] = { +20, 0, 0};
	// 0 ~ -40
	// max min(map y)
	for (int i = 0; i < mapNum[stage]; i++)
	{
		md = &mapData[i];
		imgs[i]->paint(dt, md->p + off);
	}	
#ifdef _DEBUG
	setLineWidth(2);
	for (int j = 0; j < mapNum[stage]; j++)
	{
		md = &mapData[j];
		for (int i = 0; i < md->pCount - 1; i++)
		{
			drawLine(md->point[i].x + map->off.x, md->point[i].y + map->off.y,
				md->point[i + 1].x + map->off.x, md->point[i + 1].y + map->off.y);
		}
	}
#endif // _DEBUG


}
bool ProcMap::move(iPoint mp)
{
#if 1	
	off += mp;
	if (off.x < offMin.x)
		off.x = offMin.x;
	else if (off.x > offMax.x)
		off.x = offMax.x;	

	if (off.y < offMin.y)
		off.y = offMin.y;
	else if (off.y > offMax.y)
		off.y = offMax.y;
#else
	iPoint v = mp - off;
	v /= iPointLength(v);
	v *= 100;
	
	printf("v=%f, %f\n", v.x, v.y);
	printf("mp=%f, %f\n", mp.x, mp.y);
	if (off.x < mp.x)
	{
		off += v;
		if (off.x > mp.x)
			off.x = mp.x;
	}
	else if (off.x > mp.x)
	{
		//off += v;
		//if (off.x < mp.x)
		//	off.x = mp.x;
	}

	if (off.y < offMin.y)
		off.y = offMin.y;
	else if (off.y > offMax.y)
		off.y = offMax.y;	

#endif

	return off.x == offMax.x || off.y == offMax.y;
}

bool ProcMap::clip(iPoint p)
{

	return isClipped;
}




iPoint objPosition[3] =
{
	{200, 180},  {2600, 180}, {3400, 100},
};
MapData mapData[] = {
	{
		{0, 0},
		{{1176 * 0, 170}, {155,145}, {280,175}, {415, 160}, {560, 180}, {1176 * 1, 180}},
		6,
	},
	{
		{1176, -17},
		{{1176 * 1, 180}, {1176 * 1.3, 180}, {1176 * 1.6, 180}, {1176 + 786, 180}},
		4,
	},
	{
		{1176 + 786, -17},
		{{(1176 + 786) * 1, 180}, {(1176 + 786 + 686) * 1, 180} },
		2,
	},
	{
		{1176 + 786 + 550, -17},
		{{(1176 + 786 + 550) * 1, 180}, {(1176 + 786 + 550 + 1008), 180}},
		2,
	},
	{
		{1176 + 786 + 686 + 872, -17},
		{{(1176 + 786 + 550 + 1008), 180}, {(1176 + 786 + 550 + 1008), 180}},
		2,
	},
	{
		{1176 + 786 + 686 + 872, -48},
		{{(1176 + 786 + 686) * 1, 180}, {(1176 + 786 + 550 + 1008 + 331), 180}},
		2,
	},
};
ImageInfo mapImageInfo[] =
{
	{
		"assets/Map/Map_00.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_01.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_02.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_03.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_04.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Final_%02d.png",
		4, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};

ImageInfo layerImageInfo[] =
{
	{
		"assets/map/BG_00.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/BG_01.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/BG_02.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};
ImageInfo mapObjectInfo[] =
{
	{
		"assets/map/Apple.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/Wall_%02d.png",
		3, 1.f, { -104 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		//{2238, 196}
		"assets/map/Final_%02d.png",
		3, 1.f, { -304 / 2 , -208},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};

#include "ProcField.h"

#include "ProcPlayer.h"
#include "ImgMgr.h"

int mapNum[] =
{
	3,
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
ImageInfo mapObjectInfo[];
static iImage** _mapImage = NULL;
static iImage** _layerImage = NULL;
static iImage** _mapObjectImage = NULL;
ProcMap::ProcMap(int stage)
{
	//Map
	this->stage = stage;
	maxW = 0;
	maxY = 0;
	viewChange = false;
	if (_mapImage == NULL)
 		_mapImage = createSingleImage(mapImageInfo,	mapNum[stage], this);

	imgs = new iImage * [mapNum[stage]];
	memset(imgs, 0x00, sizeof(imgs));
	for (int i = 0; i < mapNum[stage]; i++)
		imgs[i] = _mapImage[i]->clone();
	//
	
	//Layer
	if (_layerImage == NULL)
		_layerImage = createSingleImage(layerImageInfo, layerNum, this);
	imgsLayer = new iImage * [layerNum];
	memset(imgsLayer, 0x00, sizeof(imgsLayer));
	for (int i = 0; i < layerNum; i++)
	{
		imgsLayer[i] = _layerImage[i]->clone();
	}
	//
	if (_mapObjectImage == NULL)
		_mapObjectImage = createSingleImage(mapObjectInfo, layerNum, this);
	memset(imgObj, 0x00, sizeof(imgObj));
	for (int i = 0; i < 3; i++)
	{
		imgObj[i] = _mapObjectImage[i]->clone();
	}
}

ProcMap::~ProcMap()
{
	for (int i = 0; i < mapNum[stage]; i++)
	{
		delete _mapImage[i];
		delete _layerImage[i];
		delete _mapObjectImage[i];
	}
	delete _mapImage;
	delete _layerImage;
	delete _mapObjectImage;
}

void ProcMap::init(int stage)
{
	for (int i = 0; i < mapNum[stage]; i++)
	{
		Texture* t = imgs[i]->tex;
		maxW += t->width;		
	}
	printf("maxW=%d\n", maxW);
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
			if (move(iPointMake(100 * dt, 50 * dt)))
			{
				viewChange = true;			
				//bg collider -> true : block to backward
			}
		}
	}
	if (x < devSize.width / 3)
		move(iPointMake(devSize.width / 3 - x, 0));
	else if (x > devSize.width * 2 / 3)
		move(iPointMake(devSize.width * 2 / 3 - x, 0));
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
	//Obj
	imgObj[1]->stopAnimation();	
	if (getKeyStat(keyboard_space))
	{
		imgObj[1]->frame = 1;
	}
	imgObj[1]->paint(dt, iPointMake(2300 + off.x, 200 + off.y));
	drawDot(1300 + off.x, 200 + off.y);
	imgObj[2]->paint(dt, iPointMake(3805 + off.x, off.y - 32));
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
	iPoint p = off;
	off += mp;
	if (off.x < offMin.x)
		off.x = offMin.x;
	else if (off.x > offMax.x)
		off.x = offMax.x;

	if (off.y < offMin.y)
		off.y = offMin.y;
	else if (off.y > offMax.y)
		off.y = offMax.y;

	return off.x == offMax.x || off.y == offMax.y;
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
		{{200,100}},
	},
	{
		{1176, -17},
		{{1176 * 1, 180}, {1176 * 1.3, 180}, {1176 * 1.6, 180}, {1176 + 1472, 180}},
		1,
		{{200,100}},
	},
	{
		{1176 + 786, -17},
		{{(1176 + 786) * 1, 180}, {(1176 + 786) * 1.3, 180}, {(1176 + 786) * 1.6, 180}, {(1176 + 786) + 1472, 180}},
		1,
		{{200,100}},
	},
	{
		{1176 + 786 + 687, -16},
		{{(1176 + 1472) * 1, 180}, {(1176 + 1472) * 1.3, 180}, {(1176 + 1472) * 1.6, 180}, {3960, 180}},
		1,
		{{200,100}},
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
		"assets/Map/Map_4.png",
		1, 1.f, { 0 , -224},
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

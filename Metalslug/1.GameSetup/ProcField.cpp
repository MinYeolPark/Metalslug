#include "ProcField.h"

Bg::Bg()
{
	/////////////////////////////////////////////////
	//Main Layer
	/////////////////////////////////////////////////
	texs = new Texture * [cuts_Stage1];

	maxW = 0;
	int i = 0;
	bd = &bgData_Stage1[i];
	for (i = 0; i < cuts_Stage1; i++)
	{
		bd = &bgData_Stage1[i];
		Texture* t = createImage(iColor4bMake(248, 0, 248, 255),
			bd->rscName);
		maxW += t->width;// * 2
		texs[i] = t;		
	}
	offMin = iPointMake(devSize.width - maxW, 0);
	off =
	offMax = iPointZero;

	//갈 수 있는 최대 y거리	
	maxY = new int[maxW];
	for (int j = 0; j < cuts_Stage1; j++)
	{
		bd = &bgData_Stage1[j];
		for (int i = 0; i < bd->pCount - 1; i++)
		{
			iPoint sp = bd->p[i];
			iPoint ep = bd->p[1 + i];

			for (int m = sp.x; m < ep.x; m++)
			{
				float a = (m - sp.x) / (ep.x - sp.x);
				maxY[m] = sp.y + (ep.y - sp.y) * a;
			}
		}
	}

	/////////////////////////////////////////////////
	//Background layer
	/////////////////////////////////////////////////
#if 1
	bgTexs = new Texture * [layers_Stage1];

	for (int i = 0; i < layers_Stage1; i++)
	{
		bl = &bgLayer_Stage1[i];
		Texture* t = createImage(iColor4bMake(248, 0, 248, 255),
			bl->rscName);

		bgTexs[i] = t;
	}
	off = iPointZero;

#endif
}

Bg::~Bg()
{
	for (int i = 0; i < cuts_Stage1; i++)
		freeImage(texs[i]);
	delete texs;
	delete bgTexs;
	delete maxY;
}

void Bg::paint(float dt)
{
	setRGBA(1, 1, 1, 1);

	Texture* tex;
	iPoint p;
#if 1  BackGround Layer
	for (int i = 0; i < layers_Stage1; i++)
	{
		tex = bgTexs[i];
		bl = &bgLayer_Stage1[i];

		p.x = bl->offSet.x + bg->off.x * bl->rate;
		p.y = bl->offSet.y;
		while (p.x < -tex->width * bl->scale)
			p.x += tex->width * bl->scale;

		for (int j = 0; j < 2; j++)
		{
			drawImage(tex, p.x + tex->width * j * bl->scale, p.y, bl->scale, bl->scale, TOP | LEFT,
				0, 0, tex->width, tex->height, 2, 0);
		}
	}
#endif
#if 1 Main Field
	for (int i = 0; i < cuts_Stage1; i++)
	{
		tex = texs[i];
		p = off;
		drawImage(tex, p.x + tex->width * i, p.y);
	}
#endif

	//////////////////////////////////////
	//For Debug
	//////////////////////////////////////

	//#issue
	setLineWidth(2);
	for (int j = 0; j < 4; j++)
	{
		bd = &bgData_Stage1[j];
		for (int i = 0; i < bd->pCount - 1; i++)
		{
			drawLine(bd->p[i].x + bg->off.x, bd->p[i].y + bg->off.y,
				bd->p[i + 1].x + bg->off.x, bd->p[i + 1].y + bg->off.y);
		}
	}


#if 0 //Collider Render for debug
	for (int i = 0; i < cuts_Stage1; i++)
	{
		bd = &bgData_Stage1[i];
		for (int j = 0; j < sizeof(bd->bgCollider) / sizeof(BgCollider); j++)
		{
			drawRect(bd->bgCollider[j].pos.x + bg->off.x,
				bd->bgCollider[j].pos.y + bg->off.y,
				bd->bgCollider[j].size.width,
				bd->bgCollider[j].size.height);
		}
	}
#endif

	
}

iPoint Bg::move(iPoint mp)
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

	return off - p;
}


BgData bgData_Stage1[cuts_Stage1] = {
	{
		"assets/BG/BG_00.png",
		{{331 * 0, 200}, {331 * 0.3, 180}, {331 * 0.6, 200}, {331 * 1, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_01.png",
		{{331 * 1, 200}, {331 * 1.3, 180}, {331 * 1.6, 200}, {331 * 2, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_02.png",
		{{331 * 2, 200}, {331 * 2.3, 180}, {331 * 2.6, 200}, {331 * 3, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_03.png",
		{{331 * 3, 200}, {331 * 3.3, 180}, {331 * 3.6, 200}, {331 * 4, 200}},
		2.f,
	},
	{
		"assets/BG/BG_04.png",
		{{331 * 4, 200}, {331 * 4.3, 180}, {331 * 4.6, 200}, {331 * 5, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_05.png",
		{{331 * 1, 200}, {331 * 1.3, 180}, {331 * 1.6, 200}, {331 * 2, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_06.png",
		{{331 * 1, 200}, {331 * 1.3, 180}, {331 * 1.6, 200}, {331 * 2, 200}},
		4,
		1.f,
	},
	{
		"assets/BG/BG_07.png",
		{{331 * 1, 200}, {331 * 1.3, 180}, {331 * 1.6, 200}, {331 * 2, 200}},
		4,
		1.f,
	}
};
BgLayer bgLayer_Stage1[layers_Stage1] =
{
	{
		"assets/BG/BG_30.png",
		1.25f,
		{0,0},
		0.2f,
	},
	{
		"assets/BG/BG_31.png",
		1.f,
		{0,105},
		0.5f,
	},
	{
		"assets/BG/BG_32.png",
		1.f,
		{0,95},
		0.8f,
	},
};

Bg* bg;

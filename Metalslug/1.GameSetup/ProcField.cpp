#include "ProcField.h"

Bg* bg;
BgData bgData_Stage1[];
BgLayer bgLayer_Stage1[];
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
		Texture* t = createImage(iColor4fMake(248, 0, 248, 255),
			bd->rscName, i);
		maxW += t->width * tex_rate;
		texs[i] = t;		
	}
	offMin = iPointMake(devSize.width - maxW * tex_rate, 0);
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
		Texture* t = createImage(iColor4fMake(248, 0, 248, 255),
			bl->rscName, i);
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
		for (int j = 0; j < 3; j++)
		{

			p.x = bl->offSet.x * bl->rate;
			p.y = bl->offSet.y; 

			while (p.x < -tex->width * bl->scale)
				p.x += tex->width * bl->scale;
					
			drawImage(tex, p.x + tex->width * bl->scale * j , p.y, bl->scale, bl->scale, TOP | LEFT,
				0, 0, tex->width, tex->height, 2, 0);
		}
	}
#endif
#if 1 Main Field
	for (int i = 0; i < cuts_Stage1; i++)
	{
		tex = texs[i];
		p = off;
		drawImage(tex, p.x + tex->width * tex_rate * i, p.y - 200,
			tex_rate, tex_rate, TOP | LEFT,
			0, 0, tex->width, tex->height, 2, 0);
	}
#endif

	//////////////////////////////////////
	//For Debug
	//////////////////////////////////////

	//#issue	
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


BgData bgData_Stage1[] = {
	{
		"assets/BG/BG_00.png",
		{{0,400}, {340,360}, {480, 400}, {662 * 1,420}},
		4,
	},
	{
		"assets/BG/BG_01.png",
		{{662 * 1,420}, {860, 390}, {980, 420}, {662 * 2,420}},
		4,		
	},
	{
		"assets/BG/BG_02.png",
		{{662 * 2,420}, {1500,420}, {1700, 400}, {662 * 3,420}},
		4,
	},
	{
		"assets/BG/BG_03.png",
		{{662 * 3,420}, {2000,420}, {2500, 420}, {662 * 4,420}},
		4,
	},
	{
		"assets/BG/BG_04.png",
		{{662 * 4,420}, {2800,420}, {3100, 420}, {662 * 5,420}},
		4,
	},
	{
		"assets/BG/BG_05.png",
		{{662 * 5, 420}, {3500,420}, {3700, 420}, {662 * 6,420}},
		4,
	},
	{
		"assets/BG/BG_06.png",
		{{662 * 6,420}, {4200,420}, {4500, 420}, {662 * 7,420}},
		4,
	},
	{
		"assets/BG/BG_07.png",
		{{662 * 7,420}, {5000,420}, {5200, 420}, {662 * 8,420}},
		4,
	}
};
BgLayer bgLayer_Stage1[] =
{
	{
		"assets/BG/BG_30.png",
		4.f,
		{0,0},
		0.2f,
	},
	{
		"assets/BG/BG_31.png",
		4.f,
		{0,180},
		0.5f,
	},
	{
		"assets/BG/BG_32.png",
		4.f,
		{0, 150},
		0.8f,
	},
};
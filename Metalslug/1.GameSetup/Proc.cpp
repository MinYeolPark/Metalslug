#include "Proc.h"

#include "GameMgr.h"
#include "InputMgr.h"
#include "UIMgr.h"

/// UI Mgr로 이동
#include "StatusUI.h"
///
#include "Loading.h"

#include "ProcField.h"
#include "ProcBullets.h"

void loadProc()
{
	loadProcField();
	loadProcPlayer();
	loadProcBullets();
	loadProcEnemy();

	loadUI();
}
void freeProc()
{
	freeProcField();
	freeProcPlayer();
	freeProcBullets();
	freeProcEnemy();

	freeUI();
}

void drawProc(float dt)
{
	iPoint off = drawProcField(dt);
	drawProcPlayer(dt, off);
	drawProcBullets(dt, off);
	drawProcEnemy(dt, off);
	drawUI(dt, off);

	setRGBA(1, 1, 1, 1);
}

bool keyProc(iKeyState stat, iPoint p)
{
	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

///////////////////////////////////////////////////
//Field
///////////////////////////////////////////////////
void loadProcField()
{
	bg = new Bg();
}

void freeProcField()
{
	delete bg;
}

iPoint drawProcField(float dt)
{
	iPoint off = bg->off;
	bg->paint(dt);

	return off;
}

///////////////////////////////////////////////////
//Player
///////////////////////////////////////////////////

void loadProcPlayer()
{
	player = new ProcPlayer();

	player->initObj();
}

void freeProcPlayer()
{
	delete player;
}

void drawProcPlayer(float dt, iPoint off)
{
	if (player->isActive)
	{
		player->updateObj(dt);
		player->drawObj(dt, off);
	}
}


///////////////////////////////////////////////////
//Bullets
///////////////////////////////////////////////////

ProcBullets*** _bullets;
ProcBullets** bullets;
int bulletNum;
static iImage*** _imgProcBullet;
void loadProcBullets()
{
	_imgProcBullet = NULL;

	_bullets = new ProcBullets * *[BulletIndexMax];
	for (int i = 0; i < BulletIndexMax; i++)
	{
		_bullets[i] = new ProcBullets * [bulletMax];
		for (int j = 0; j < bulletMax; j++)
			_bullets[i][j] = new ProcBullets(i);
	}
	bullets = new ProcBullets * [BulletIndexMax * bulletMax];
	bulletNum = 0;
}

void freeProcBullets()
{
	for (int i = 0; i < bulletMax; i++)
	{
		delete _imgProcBullet[i];
		delete _bullets[i];
	}
	delete _imgProcBullet;
	delete _bullets;

	delete bullets;
	_imgProcBullet = NULL;
}

void drawProcBullets(float dt, iPoint off)
{
	for (int i = 0; i < bulletNum; i++)
	{
		ProcBullets* b = bullets[i];
		b->updateObj(dt);
		b->drawObj(dt, off);
	}
}

void addBullet(ProcObject* parent, int idx, int dir)
{
#if 1
	for (int i = 0; i < bulletMax; i++)
	{
		printf("bulletNum[%d]\n", bulletNum);
		ProcBullets* b = _bullets[idx][i];
		if (b->isActive == false)
		{
			b->isActive = true;
			b->p = iPointMake(100, 100);
			bullets[bulletNum] = b;
			bulletNum++;
			return;

		}
#if 0
#if 0
			b->p = parent->p + iPointRotate(parent->firePoint, iPointZero, 360 - d[dir]);
#else
			if (parent->layer == OBJECT_LAYER::Player)
			{
				ProcPlayer* own = (ProcPlayer*)parent;
				float y = own->firePoint.y;
				own->firePoint.y = 0;
				iPoint mp = iPointRotate(own->firePoint, iPointZero, 360 - d[dir]);
				mp.y += y;
				own->firePoint.y = y;
				b->p = parent->p + mp;
			}
			if (parent->layer == OBJECT_LAYER::Enemy)
				parent = (ProcEnemy*)parent;
#endif
			b->parent = parent;
			b->v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - d[dir]);

		}
#endif
	}
#endif
}

///////////////////////////////////////////////////
//UI
///////////////////////////////////////////////////
iStrTex* stPlaytime;
igImage** igNumber;
Texture* methodStPlaytime(const char* str);

void loadUI()
{
	status = new StatusUI();

	iGraphics* g = new iGraphics();
	stPlaytime = new iStrTex(methodStPlaytime);
	igNumber = new igImage * [10];
	for (int i = 0; i < 10; i++)
		igNumber[i] = g->createIgImage(	"Resources/NumFont/NumFont_%02d.png", i);

	delete g;
}
void freeUI()
{
#if 1
	delete status;
#endif
	delete stPlaytime;
	iGraphics* g = new iGraphics();
	for (int i = 0; i < 10; i++)
		g->freeIgImage(igNumber[i]);
	delete igNumber;
	delete g;
}
void drawUI(float dt, iPoint off)
{
	status->paint(dt);

	playtime += dt;
	stPlaytime->setString("%.0f", playtime);
	Texture* t = stPlaytime->tex;
	drawImage(t, devSize.width / 2, 16, 2, 2, TOP | HCENTER, 0, 0, t->width, t->height, 2, 0);
}
void addUI(iPoint p, int num)
{

}
Texture* methodStPlaytime(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, 32);
	g->init(size);

	//setRGBA(1, 1, 1, 0.3f);
	//g->fillRect(0, 0, size.width, size.height);
	//setRGBA(1, 1, 1, 1);
#if 0
	setStringName("궁서체");
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%ss", str);
#else
	int i, j = strlen(str);
	iPoint p = iPointZero;
#if 1// hcenter
	int w = 0;
	for (int i = 0; i < j; i++)
	{
		igImage* ig = igNumber[str[i] - '0'];
		w += ig->GetWidth() + 1;
	}
	p.x -= w / 2;
#endif
	for (int i = 0; i < j; i++)
	{
		igImage* ig = igNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}
#endif
	Texture* tex = g->getTexture(iColor4bMake(163, 73, 164, 255));
	delete g;

	return tex;
}
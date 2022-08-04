#include "StatusUI.h"

#include "GameMgr.h"
#include "UIMgr.h"
#include "ProcPlayer.h"

StatusUI* status;
Texture* methodStPlaytime(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, 32);

	g->init(size);
#if 0
	setStringName("±Ã¼­Ã¼");
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%ss", str);
#else
	int i, j = strlen(str);
	iPoint p = iPointZero;		
#if 0// hcenter
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
		igImage* ig = boldNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}
#endif
	Texture* tex = g->getTexture(iColor4bMake(163, 73, 164, 255));
	delete g;

	return tex;
}

Texture* methodStScore(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, 32);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	iSize s = iSizeMake(16, 16);
	for (int i = 0; i < j; i++)
	{
		igImage* ig = normNumber[str[i] - '0'];

		//#issue
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

Texture* methodStAmmo(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(48, 16);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	for (int i = 0; i < j; i++)
	{
		igImage* ig = goldNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}
Texture* methodStBomb(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(32, 16);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	iSize s = iSizeMake(16, 16);
	for (int i = 0; i < j; i++)
	{
		igImage* ig = goldNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

Texture* methodStLife(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(16, 16);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	iSize s = iSizeMake(16, 16);
	for (int i = 0; i < j; i++)
	{
		igImage* ig = goldNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}


StatusUI::StatusUI()
{
	setRGBA(1, 1, 1, 1);
	iGraphics* g = new iGraphics();
	iSize s = iSizeMake(32, 16);
	g->init(s);
	g->drawIgImage(goldNumber[10], iPointZero, 0);
	inf = g->getTexture();

	num = 0; p = iPointZero; delta = 0.f;
	for (int i = 0; i < 3; i++)
		statsFrame[i] = createImage(iColor4bMake(222, 0, 255, 255), "assets/NumFont/ScoreFrame_00.png");
	for (int i = 0; i < 4; i++)
		gaugeFrame[i] = createImage(iColor4bMake(222, 0, 255, 255), "assets/NumFont/GaugeFrame_%02d.png", i);
	stPlaytime = new iStrTex(methodStPlaytime);
	stScore = new iStrTex(methodStScore);
	stAmmo = new iStrTex(methodStAmmo);
	stBomb = new iStrTex(methodStBomb);
	stLife = new iStrTex(methodStLife);
	delete g;
}

StatusUI::~StatusUI()
{	
	delete stPlaytime;
	delete stScore;
	delete stAmmo;
	delete stBomb;	
	delete stLife;

	delete inf;
	delete statsFrame;
	delete gaugeFrame;
}

bool StatusUI::paint(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawImage(statsFrame[0], devSize.width/4, 12);
	drawImage(gaugeFrame[0], devSize.width/16, 12);

	stScore->drawString(devSize.width / 6, 2, TOP | LEFT, "%d", player->score);
	stLife->drawString(devSize.width / 6 + 3, 22, TOP | LEFT, "%d", player->life);
	if (player->curGun->gunIndex == Handgun)
		drawImage(inf, 90, 22, 0.5, 0.5);
	else
		stAmmo->drawString(105, 29, 0.9, 0.9, VCENTER | HCENTER, "%d", player->curGun->remain);
	stBomb->drawString(130, 29, 0.9, 0.9, VCENTER | HCENTER, "%d", player->bombs);
	stPlaytime->drawString(devSize.width / 2, 16, TOP | LEFT, "%.0f", playtime);
	playtime += dt;
	//Texture* t = stPlaytime->tex;
	//drawImage(t, devSize.width / 2, 16, TOP | LEFT);
	setRGBA(1, 1, 1, 1);
	return true;
}

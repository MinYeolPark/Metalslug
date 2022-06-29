#include "StatusUI.h"

#include "GameMgr.h"
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
		igImage* ig = status->igNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}
#endif
	Texture* tex = g->getTexture(iColor4bMake(163, 73, 164, 255));
	delete g;

	return tex;
}
Texture* methodStSetting(const char* str, void* parm)
{
	return NULL;
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
		igImage* ig = status->number[str[i] - '0'];

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
	iSize size = iSizeMake(16, 16);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	iSize s = iSizeMake(16, 16);
	for (int i = 0; i < j; i++)
	{
		igImage* ig = status->numberGold[str[i] - '0'];
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
		igImage* ig = status->numberGold[str[i] - '0'];
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
		igImage* ig = status->numberGold[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}


StatusUI::StatusUI()
{
	iGraphics* g = new iGraphics();
	stPlaytime = new iStrTex(methodStPlaytime);

	num = 0; p = iPointZero; delta = 0.f;
	alphabet = new igImage * [26];
	number = new igImage * [10];
	numberGold = new igImage * [10];

	igNumber = new igImage * [10];
	for (int i = 0; i < 10; i++)
		igNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i);

	for (int i = 0; i < 26; i++)
		alphabet[i] = g->createIgImage("assets/NumFont/AlphabetFont_%02d.png", i + 1);
	for (int i = 0; i < 10; i++)
		number[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 10);
	for (int i = 0; i < 10; i++)
		numberGold[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 20);


	for (int i = 0; i < 3; i++)
		statsFrame[i] = createImage(iColor4bMake(222, 0, 255, 255), "assets/NumFont/ScoreFrame_%02d.png", i);
	for (int i = 0; i < 4; i++)
		gaugeFrame[i] = createImage(iColor4bMake(222, 0, 255, 255), "assets/NumFont/GaugeFrame_%02d.png", i);
	stScore = new iStrTex(methodStScore);
	stAmmo = new iStrTex(methodStAmmo);
	stBomb = new iStrTex(methodStBomb);
	stLife = new iStrTex(methodStLife);
	delete g;
}

StatusUI::~StatusUI()
{
	iGraphics* g = new iGraphics();
	for (int i = 0; i < 10; i++)
		g->freeIgImage(igNumber[i]);

	for (int i = 0; i < 10; i++)
		g->freeIgImage(alphabet[i]);

	for (int i = 0; i < 10; i++)
		g->freeIgImage(number[i]);

	delete igNumber;

	delete stPlaytime;
	delete stScore;
	delete stAmmo;
	delete stBomb;
	delete status;
	delete g;
}

bool StatusUI::paint(float dt)
{
	drawImage(statsFrame[0], devSize.width/4, 12, 1.1, 1.1);
	drawImage(gaugeFrame[0], devSize.width/16, 12);

	stScore->drawString(devSize.width / 6, 2, TOP | LEFT, "%d", player->score);
	stLife->drawString(devSize.width / 6, 22, TOP | LEFT, "%d", player->life);
	stAmmo->drawString(115, 31, VCENTER | HCENTER, "%d", player->curGun->remain);
	stBomb->drawString(145, 31, VCENTER | HCENTER, "%d", player->bombs);

	playtime += dt;
	stPlaytime->setString("%.0f", playtime);
	Texture* t = stPlaytime->tex;
	drawImage(t, devSize.width / 2, 16, TOP | LEFT);
	return true;
}

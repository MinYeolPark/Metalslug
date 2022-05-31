#include "StatusUI.h"

#include "ProcPlayer.h"

Texture* methodStSetting(const char* str, void* parm)
{
	return NULL;
}
Texture* methodStScore(const char* str)
{
	printf("methodSt Score!\n");
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, 32);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	iSize s = iSizeMake(16, 16);
	for (int i = 0; i < j; i++)
	{
		igImage* ig = status->number[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

Texture* methodStAmmo(const char* str)
{
	return methodStSetting(str, 0);
}
Texture* methodStBomb(const char* str)
{
	return methodStSetting(str, 0);
}

Texture* methodStLife(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, 32);
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


StatusUI* status;
StatusUI::StatusUI()
{
	iGraphics* g = new iGraphics();
	num = 0; p = iPointZero; delta = 0.f;
	alphabet = new igImage * [26];
	number = new igImage * [10];
	numberGold = new igImage * [10];

	for (int i = 0; i < 26; i++)
		alphabet[i] = g->createIgImage("Resources/NumFont/AlphabetFont_%02d.png", i + 1);
	for (int i = 0; i < 10; i++)
		number[i] = g->createIgImage("Resources/NumFont/NumFont_%02d.png", i + 10);
	for (int i = 0; i < 3; i++)
		statsFrame[i] = createImage("Resources/NumFont/ScoreFrame_%02d.png", i);
	for (int i = 0; i < 4; i++)
		gaugeFrame[i] = createImage("Resources/NumFont/GaugeFrame_%02d.png", i);
	for (int i = 0; i < 10; i++)
		numberGold[i] = g->createIgImage("Resources/NumFont/NumFont_%02d.png", i + 20);

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
		g->freeIgImage(alphabet[i]);
	for (int i = 0; i < 10; i++)
		g->freeIgImage(number[i]);
	delete g;

	delete stScore;
	delete stAmmo;
	delete stBomb;
}

bool StatusUI::paint(float dt)
{
	drawImage(statsFrame[0], 160, 16, 2, 2);
	drawImage(gaugeFrame[0], 40, 16, 2, 2);

	stScore->drawString(130, 2, 2, 2, TOP | LEFT, "%d", player->hp);
	//stAmmo->drawString(100, 0, TOP | LEFT, "%.0f", 10);
	//stBomb->drawString(200, 0, 2, 2, TOP | LEFT, "%.0f", player->hp);
	stLife->drawString(110, 36, 2, 2, TOP | LEFT, "%d", player->life);
	return true;
}

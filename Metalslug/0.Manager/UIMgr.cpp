#include "UIMgr.h"

#include "iOpenAL.h"

#include "GameMgr.h"
#include "ImgMgr.h"

#include "Menu.h"
#include "Loading.h"
#include "StatusUI.h"
#include "PopupUI.h"

AudioInfo ai[8] = {
	{ (char*)"assets/sound/pop_close.wav", false, 1.0f },
	{ (char*)"assets/sound/pop_open.wav", false, 1.0f },	
	{ (char*)"assets/sound/nar_m1str.wav", false, 1.0f },
	{ (char*)"assets/sound/fire.wav", false, 1.0f },
	{ (char*)"assets/sound/select_eri.wav", false, 1.0f },
	{ (char*)"assets/sound/title.wav", false, 1.0f },
	{ (char*)"assets/sound/select.wav", true, 1.0f },
	{ (char*)"assets/sound/stage1.wav", true, 1.0f },
};

bool isPopup;
iStrTex* stPlaytime;

igImage** boldNumber;
igImage** normNumber;
igImage** goldNumber;
igImage** bigNumber;
igImage** normAlphabet;
void loadUI()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_title);	

	isPopup = false;
	iGraphics* g = new iGraphics();

	boldNumber = new igImage * [10];
	normNumber = new igImage * [10];
	goldNumber = new igImage * [10];
	bigNumber = new igImage * [10];
	normAlphabet = new igImage * [26];

	memset(boldNumber, 0x00, sizeof(boldNumber));
	memset(boldNumber, 0x00, sizeof(normNumber));
	memset(boldNumber, 0x00, sizeof(goldNumber));
	memset(boldNumber, 0x00, sizeof(bigNumber));
	memset(boldNumber, 0x00, sizeof(normAlphabet));

	for (int i = 0; i < 10; i++)
	{
		boldNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i);
		normNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 10);
		goldNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 20);
		bigNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 30);
	}
	for (int j = 0; j < 26; j++)
		normAlphabet[j] = g->createIgImage("assets/NumFont/AlphabetFont_%02d.png", j + 1);
#if 0
#endif
	status = new StatusUI();
	popup = new PopupUI();
#if true
	createConfirmPopup();
	createCountdownPopup();
#endif
}

void freeUI()
{
	freeConfirmPopup();
	freeCountdownPopup();
#if 1
	for (int i = 0; i < 10; i++)
	{
		delete boldNumber[i];
		delete normNumber[i];
		delete goldNumber[i];
		delete bigNumber[i];
	}
	for (int j = 0; j < 26; j++)
		delete normAlphabet[j];
	delete boldNumber;
	delete normNumber;
	delete goldNumber;
	delete bigNumber;
	delete normAlphabet;

	boldNumber = NULL;
	normNumber = NULL;
	goldNumber = NULL;
	bigNumber = NULL;
	normAlphabet = NULL;

	delete status;
	delete popup;
#endif
}

void drawUI(float dt, iPoint off)
{	
	setRGBA(1, 1, 1, 1);
	status->paint(dt);	
	popup->paint(dt);
#if 1
	drawConfirmPopup(dt);
	drawSettingPopup(dt);
	drawCountdownPopup(dt);
#endif

	setRGBA(1, 1, 1, 1);
}

bool keyProcUI(iKeyState stat, iPoint p)
{
	if (popup->popOption->bShow == false)
		return false;
	if (popup->popOption->state != iPopupStateProc)
		return true;

	if (keySettingPopup(stat, p))
		return true;

	int i, j = -1;
	switch (stat) {
	case iKeyStateBegan:
		i = popup->popOption->selected;		
		if (i == -1)break;
		if (i == 0)
		{			
			optionConfirm = true;	
		}
		else if (i == 1)
		{
			optionConfirm = true;
			showPopSetting(true);
		}
		else if (i == 2)
		{
			optionConfirm = true;
			setLoading(GameStateMenu, freeProc, loadMenu);
		}

		break;

	case iKeyStateMoved:
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(p, popup->imgOptionBtn[i]->rect()))
			{
				j = i;
				break;
			}
		}
		if (selectedOptionBtn!=j)
		{
			selectedOptionBtn = j;
			popup->popOption->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

iPopup* confirmPopup;
iImage** confirmBtn;
void drawConfrimBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 2; i++)
		confirmBtn[i]->frame = (i == confirmPopup->selected);
}

void createConfirmPopup()
{
	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(70, 50);
	g->init(size);

	igImage* i = g->createIgImage("assets/UI/Confirm.png");
	g->drawIgImage(i, 0, 0, 1.2, 1.2, TOP | LEFT);
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);

	img->position = iPointMake(devSize.width / 2 - i->GetWidth() / 2, devSize.height / 2);
	freeImage(tex);

	pop->addObject(img);
	//
	// imgTopMenuBtn[3]
	//
	const char* str[2] = { "YES", "NO" };
	size = iSizeMake(30, 10);
	confirmBtn = new iImage * [2];	
	for (int i = 0; i < 2; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			iPoint pos = iPointZero;
			g->init(size);
			if (j == 0)
				setRGBA(1, 1, 1, 1);
			else
				setRGBA(1, 1, 0, 1);
			for (int k = 0; k < strlen(str[i]); k++)
			{
				igImage* ig = normAlphabet[str[i][k] - 'A'];
				g->drawIgImage(ig, pos.x + 5, pos.y + 1, TOP | LEFT);
				pos.x += ig->GetWidth() + 1;
			}
			if (j == 0)
				setRGBA(1, 1, 1, 0);
			else// if (j == 1)
				setRGBA(1, 1, 0, 0.4);
			g->fillRect(0, 0, size.width, size.height, 4);
			setRGBA(1, 1, 1, 1);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width / 2 - size.width / 2, 130 + 10 * i);
		pop->addObject(img);
		confirmBtn[i] = img;
	}
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = drawConfrimBefore;
	confirmPopup = pop;
	confirmPopup->selected = 0;
	confirmPopup->show(false);
	delete g;
}
void freeConfirmPopup()
{
}
void drawConfirmPopup(float dt)
{
	confirmPopup->paint(dt);
}
bool keyConfirmPopup(iKeyState stat, iPoint p)
{
	if (confirmPopup->bShow == false)
		return false;
	if (confirmPopup->state != iPopupStateProc)
		return true;
	int i, j = -1;
	switch (stat) {
	case iKeyStateBegan:
		i = confirmPopup->selected;
		if (i == -1) break;
		if (i == 0)
		{
			printf("yes\n");
		}
		else if (i == 1)
			printf("No\n");
	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, confirmBtn[i]->rect(confirmPopup->closePoint)))
			{
				j = i;
				printf("%d\n", j);
				break;
			}
		}
		if (j != confirmPopup->selected)
			confirmPopup->selected = j;
		break;
	case iKeyStateEnded:
		break;
	}
	return true;
}

iPopup* countDownPopup;
iStrTex* stCount;
int countDown;
Texture* methodPopcount(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(48, 48);
	g->init(size);

	int i, j = strlen(str);
	iPoint p = iPointZero;
	for (int i = 0; i < j; i++)
	{
		igImage* ig = bigNumber[str[i] - '0'];
		g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
		p.x += ig->GetWidth() + 1;
	}

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}
void createCountdownPopup()
{
	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 2, devSize.height / 2);
	g->init(size);	
	//stCount = new iStrTex(methodPopcount);	
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);

	img->position = iPointZero;
	freeImage(tex);

	pop->addObject(img);
	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;	
	countDownPopup = pop;
	countDownPopup->selected = 0;
	countDownPopup->show(false);
}

void freeCountdownPopup()
{
}

void drawCountdownPopup(float dt)
{
	countDownPopup->paint(dt);
}

void showPopCountdown(bool show)
{
	countDownPopup->show(show);
}
#include "UIMgr.h"

#include "iOpenAL.h"

#include "GameMgr.h"
#include "ImgMgr.h"

#include "Menu.h"
#include "Loading.h"
#include "StatusUI.h"
#include "PopupUI.h"

#if 0
AudioInfo ai[5] =
{
	{ (char*)"assets/sound/TITLE.wav", false, 1.0f },

enum
{
	snd_title = 0,

	snd_eff_max,

	snd_bgm_title = snd_eff_max,
	snd_bgm_map,
	snd_bgm_song,

	snd_max,
};
#endif
AudioInfo ai[5] = {
	{ (char*)"assets/sound/btn0.wav", false, 1.0f },
	{ (char*)"assets/sound/btn1.wav", false, 1.0f },	
	{ (char*)"assets/sound/bgm0.wav", true, 0.4f },
	{ (char*)"assets/sound/bgm1.wav", true, 1.0f },
	{ (char*)"assets/sound/bgm2.wav", true, 1.0f },
};
enum {
	snd_btn_sel = 0,
	snd_btn_ok,

	snd_eff_max,

	snd_bgm_title = snd_eff_max,
	snd_bgm_map,
	snd_bgm_song,

	snd_max,
};
bool isPopup;
iStrTex* stPlaytime;

igImage** boldNumber;
igImage** normNumber;
igImage** goldNumber;
igImage** normAlphabet;
void loadUI()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_song);

	isPopup = false;
	iGraphics* g = new iGraphics();


	boldNumber = new igImage * [10];
	normNumber = new igImage * [10];
	goldNumber = new igImage * [10];
	normAlphabet = new igImage * [26];

	memset(boldNumber, 0x00, sizeof(boldNumber));
	memset(boldNumber, 0x00, sizeof(normNumber));
	memset(boldNumber, 0x00, sizeof(goldNumber));
	memset(boldNumber, 0x00, sizeof(normAlphabet));

	for (int i = 0; i < 10; i++)
	{
		boldNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i);
		normNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 10);
		goldNumber[i] = g->createIgImage("assets/NumFont/NumFont_%02d.png", i + 20);
	}
	for (int j = 0; j < 26; j++)
		normAlphabet[j] = g->createIgImage("assets/NumFont/AlphabetFont_%02d.png", j + 1);
#if 0
#endif
	status = new StatusUI();
	popup = new PopupUI();
}

void freeUI()
{
#if 1
	for (int i = 0; i < 10; i++)
	{
		delete boldNumber[i];
		delete normNumber[i];
		delete goldNumber[i];
	}
	for (int j = 0; j < 26; j++)
		delete normAlphabet[j];
	delete boldNumber;
	delete normNumber;
	delete goldNumber;
	delete normAlphabet;

	boldNumber = NULL;
	normNumber = NULL;
	goldNumber = NULL;
	normAlphabet = NULL;

	delete status;
	delete popup;
#endif
}

void drawUI(float dt, iPoint off)
{	
	status->paint(dt);	
	popup->paint(dt);
}

bool keyProcUI(iKeyState stat, iPoint p)
{
	if (popup->popOption->bShow == false)
		return false;
	if (popup->popOption->state != iPopupStateProc)
		return true;

	int i, j = -1;
	switch (stat) {
	case iKeyStateBegan:
		i = popup->popOption->selected;
		printf("isSelectedindex=?%d\n", selectedOptionBtn);
		if (i == -1)break;
		if (i == 0)
		{			
			optionConfirm = true;	
		}
		else if (i == 1)
		{
			optionConfirm = true;
		}
		else if (i == 2)
		{
			optionConfirm = true;
			setLoading(GameStateMenu, freeProc, loadMenu);
		}

		if (optionConfirm == true)
		{
			printf("optionConfirmed, index=%d\n", i);
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

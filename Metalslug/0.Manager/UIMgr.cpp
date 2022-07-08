#include "UIMgr.h"

#include "GameMgr.h"
#include "ImgMgr.h"

#include "StatusUI.h"
#include "PopupUI.h"

bool isPopup;
iStrTex* stPlaytime;

igImage** boldNumber;
igImage** normNumber;
igImage** goldNumber;
igImage** normAlphabet;
void loadUI()
{
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
	if (popup->popOption->bShow)
		isPopup = true;
}

void addUI(iPoint p, int num)
{

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
		printf("Key ui\n");
		i = popup->popOption->selected;
		if (i == -1)break;
		break;

	case iKeyStateMoved:
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(p, popup->imgPauseBtn[i]->rect(popup->popOption->closePoint)))
			{
				j == i;
				break;
			}
		}
		if (j != popup->popOption->selected)
		{
			popup->popOption->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

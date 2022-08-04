#include "Menu.h"

#include "Loading.h"
#include "Proc.h"
#include "Select.h"

#include "UIMgr.h"

#include "App.h"
Texture* titlemap;
iPopup* titleMenu;
iImage** titleMenuBtn;
void drawMenuBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 4; i++)
		titleMenuBtn[i]->frame = (i == titleMenu->selected);
}

void loadMenu()
{
	audioPlay(snd_bgm_title);
	audioStop(snd_bgm_select);
	audioStop(snd_bgm_stage1);
	audioStop(snd_bgm_boss);
	audioStop(snd_bgm_clear);

	titlemap = createImage("assets/map/Title.png");	
	createTitleMenu();	
	titleMenu->show(true);
}

void freeMenu()
{
	freeTitleMenu();	
}

void drawMenu(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawImage(titlemap, 0, 0,
		devSize.width / titlemap->width,
		devSize.height / titlemap->height, TOP | LEFT);
	drawConfirmPopup(dt);
	drawTitleMenu(dt);
	setRGBA(1, 1, 1, 1);
}

void keyMenu(iKeyState stat, iPoint p)
{
	if (keyTitleMenu(stat, p))
		return;
	else if (keyConfirmPopup(stat, p))
		return;
}

void createTitleMenu()
{
#if 1
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width / 3, devSize.height / 3);
	g->init(size);	
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	//
	// imgTopMenuBtn[3]
	//
	const char* str[4] =
	{ "GAMESTART","DISPLAY", "OPTION","EXIT" };
	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(20);
	setStringBorder(2);

	size = iSizeMake(120, 20);
	titleMenuBtn = new iImage * [4];
	for (int i = 0; i < 4; i++)
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
				pos.y = i ;
				g->drawIgImage(ig, pos.x + 20, pos.y + 4, TOP | LEFT);
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
		img->position = iPointMake(devSize.width / 2 - size.width / 2,
			(devSize.height / 2 + 20 * i));
		pop->addObject(img);
		titleMenuBtn[i] = img;
	}
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleMove;
	pop->openPoint = iPointMake(0, -30);
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = drawMenuBefore;
	titleMenu = pop;
	titleMenu->selected = 0;
	delete g;
#endif
}

void freeTitleMenu()
{
	delete titleMenu;
	delete titleMenuBtn;
}

#include "InputMgr.h"
void drawTitleMenu(float dt)
{
	titleMenu->paint(dt);
	if (getKeyDown(keyboard_down))
	{
		titleMenu->selected++;
		if (titleMenu->selected > 3)
			titleMenu->selected = 0;
	}
	if (getKeyDown(keyboard_up))
	{
		titleMenu->selected--;
		if (titleMenu->selected < 0)
			titleMenu->selected = 3;
	}
	if (getKeyDown(keyboard_enter))
	{		
		if (titleMenu->selected == 0)
			setLoading(GameStateSelect, freeMenu, loadSelect);
		else if (titleMenu->selected == 1)
			;
		else if (titleMenu->selected == 2)
			;
		else if (titleMenu->selected == 3)
		{
			int result = MessageBox(NULL, L"Quit?", L"Exit", MB_YESNO);
			if (result == IDYES)
				runApp = false;
			else
				return;			
		}
	}
}
bool keyTitleMenu(iKeyState stat, iPoint p)
{
	if (titleMenu->bShow == false)
		return false;
	if (titleMenu->state != iPopupStateProc)
		return true;
	int i, j = -1;
	switch (stat) {
	case iKeyStateBegan:
		i = titleMenu->selected;		
		if (i == -1) break;
		if (i == 0)
		{
			setLoading(GameStateSelect, freeMenu, loadSelect);
		}
		else if (i == 1)
			printf("Display\n");
		else if (i == 2)
			printf("Option\n");
		else if (i == 3)
		{
			int result = MessageBox(NULL, L"Quit?", L"Exit", MB_YESNO);
			if (result == IDYES)
				runApp = false;
			else
				return false;
		}
	case iKeyStateMoved:
		for (i = 0; i < 4; i++)
		{
			if (containPoint(p, titleMenuBtn[i]->rect(titleMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != titleMenu->selected)
			titleMenu->selected = j;
		break;
	case iKeyStateEnded:
		break;
	}
	return true;
}

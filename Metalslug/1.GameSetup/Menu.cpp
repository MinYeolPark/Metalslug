#include "Menu.h"

#include "Loading.h"
#include "Proc.h"
#include "Select.h"

#include "iPopup.h"

Texture* titleBg;
iPopup* titleMenu;
iImage** titleMenuBtn;
void drawMenuBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 4; i++)
		titleMenuBtn[i]->frame = (i == titleMenu->selected);
}
void loadMenu()
{
	titleBg = createImage("assets/BG/Title.png");

	createTitleMenu();

	showTitleMenu(true);
}

void freeMenu()
{

}

void drawMenu(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawImage(titleBg, 0, 0,
		devSize.width / titleBg->width,
		devSize.height / titleBg->height, TOP | LEFT);

	drawTitleMenu(dt);
	setRGBA(1, 1, 1, 1);
}

void keyMenu(iKeyState stat, iPoint p)
{
	if (keyTitleMenu(stat, p))
		return;
}


void createTitleMenu()
{	
#if 1
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	
	iSize size = iSizeMake(devSize.width/3, devSize.height/3);	
	g->init(size);

	setRGBA(1, 0, 1, 0.5f);
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);	
	freeImage(tex);

	pop->addObject(img);

	//
	// imgTopMenuBtn[3]
	//
	const char* str[4] =
	{ "Game Start","Map Tool", "Option","Exit" };
	setStringName("assets/BMJUA_ttf.ttf");	
	setStringSize(20);
	setStringBorder(2);
	
	size = iSizeMake(160, 25);
	titleMenuBtn = new iImage * [4];
	for (int i = 0; i < 4; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			if (j == 0)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
				setStringRGBA(0, 0, 0, 1);
			}
			else
			{
				setRGBA(1, 1, 0, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			g->fillRect(0, 0, size.width, size.height, 1);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width / 2 - size.width / 2,
			(devSize.height / 2 + 30 * i));		
		pop->addObject(img);
		titleMenuBtn[i] = img;
	}
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleMove;
	pop->openPoint = iPointMake(0, -30);
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = drawMenuBefore;
	titleMenu = pop;

	delete g;
#endif
}
void freeTitleMenu()
{
	delete titleMenu;
	delete titleMenuBtn;
}
void drawTitleMenu(float dt)
{	
	titleMenu->paint(dt);
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
			printf("GameStart\n");
#if 0
			setLoading(GameStateProc, freeMenu, loadProc);
#else
			setLoading(GameStateSelect, freeMenu, loadSelect);
#endif
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
void showTitleMenu(bool show)
{
	titleMenu->show(show);
}
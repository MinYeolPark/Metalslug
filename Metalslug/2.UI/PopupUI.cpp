#include "PopupUI.h"

#include "UIMgr.h"
#include "InputMgr.h"

PopupUI* popup;
igImage* bg;
int selectedOptionBtn;
bool optionConfirm = false;
PopupUI::PopupUI()
{
	setRGBA(1, 1, 1, 1);
	iPopup* pop = new iPopup();	
    iGraphics* g = new iGraphics();	
    iSize size = iSizeMake(150, 100);
    g->init(size);	

	bg = g->createIgImage("assets/UI/Option.png");    
	iPoint p = iPointZero;		
	g->drawIgImage(bg, p.x, p.y, 1.2, 1.2, TOP | LEFT);
    Texture* tex = g->getTexture();
    iImage* img = new iImage();
    img->addObject(tex);

	img->position = iPointMake(devSize.width / 2 - bg->GetWidth() / 2, devSize.height / 3);
    freeImage(tex);
	pop->addObject(img);
	//
	// btn (80, 20) 
	//
	const char* str[3] = { "RESUME", "SETTING", "QUIT" };
	imgOptionBtn = new iImage * [3];
	size = iSizeMake(80, 15);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringRGBA(1, 1, 1, 1);
	setStringSize(10);
	setStringBorder(0);
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			iPoint pos = iPointZero;
			g->init(size);			
			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
			}
			else// if (j == 1)
			{
				setRGBA(1, 1, 0, 1);
			}			
			for (int k = 0; k < strlen(str[i]); k++)
			{
				igImage* ig = normAlphabet[str[i][k] - 'A'];
				pos.y = i * 1;
				g->drawIgImage(ig, pos.x, pos.y + 4, TOP | LEFT);				
				pos.x += ig->GetWidth() + 1;
			}	
			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}		

		img->position = iPointMake((devSize.width / 2 - bg->GetWidth() / 2)+ 20,
			(devSize.height / 3 + 15) + 12 * i);
		pop->addObject(img);		
		imgOptionBtn[i] = img;
	}
	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointMake(0, 0);
	pop->closePoint = iPointMake(0, 0);

	void drawPopOptionBefore(float dt, iPopup * pop);
	pop->methodDrawBefore = drawPopOptionBefore;
	popOption = pop;

	popOption->show(false);

#if 1
	createSettingPopup();
#endif
	setRGBA(1, 1, 1, 1);
}

PopupUI::~PopupUI()
{
}

bool PopupUI::paint(float dt)
{	
	setRGBA(1, 1, 1, 1);
	popOption->paint(dt);

	bool show = popOption->bShow;
	if (getKeyDown(keyboard_esc))
	{
		show = !show;
		popOption->show(show);
	}
	setRGBA(1, 1, 1, 1);
    return false;
}

void drawPopOptionBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		popup->imgOptionBtn[i]->frame = (i == popup->popOption->selected);
}


// -----------------------------------
// popSetting
// -----------------------------------
iPopup* popSetting;
iImage** imgSettingBtn;
float rangeBtn[2];// min / max about x
void drawSettingBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		imgSettingBtn[i]->frame = (i == popSetting->selected);
}
void createSettingPopup()
{
	setRGBA(1, 1, 1, 1);
	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(200, 200);
	g->init(size);
	
	setRGBA(1, 1, 1, 1);
	iPoint p = iPointZero;
	g->drawIgImage(bg, p.x, p.y, 1.5, 1.5, TOP | LEFT);	
	for (int i = 0; i < 2; i++)
		g->fillRect(45, 35 + 20 * i, 105, 2, 1);
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	img->position = iPointZero; 
	freeImage(tex);
	pop->addObject(img);

#if 1
	const char* strOption[2] = { "BGM", "SFX" };
	imgSettingBtn = new iImage * [2];
	size = iSizeMake(30, 40);
	for (int j = 0; j < 2; j++)
	{
		img = new iImage();
		iPoint pos = iPointZero;
		g->init(size);
		setRGBA(1, 1, 1, 1);
		for (int i = 0; i < strlen(strOption[j]); i++)
		{
			igImage* ig = normAlphabet[strOption[j][i] - 'A'];
			pos.y = j * 20;
			g->drawIgImage(ig, pos.x, pos.y, TOP | LEFT);
			pos.x += ig->GetWidth() + 1;
		}
		tex = g->getTexture();
		img->addObject(tex);

		img->position = iPointMake(tex->potWidth / 4, tex->potHeight / 2);
		freeImage(tex);
		
		pop->addObject(img);
		imgSettingBtn[j] = img;
	}
	// 
	// btn
	//
	imgSettingBtn = new iImage * [3];
	// [0] -    [1] - clone
	img = new iImage();
	size = iSizeMake(10, 10);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0) 
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(1, 1, 1, 0.5f);
		g->fillRect(0, 0, size.width, size.height, size.width / 2);
		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(145-5, 35-5);
	pop->addObject(img);
	imgSettingBtn[0] = img;

	img = img->clone();
	img->position.y += 20;
	pop->addObject(img);
	imgSettingBtn[1] = img;

	rangeBtn[0] = 45 - 5;
	rangeBtn[1] = rangeBtn[0] + 105;

	// [2]
	img = new iImage();
	size = iSizeMake(35, 10);
	for (int j = 0; j < 2; j++)
	{
		g->init(size);

		if (j == 0)
		{
			setRGBA(0.5, 0.5, 0.5, 1);
			setStringRGBA(0, 0, 0, 1);
		}
		else// if (j == 1)
		{
			setRGBA(1, 1, 0, 1);
			setStringRGBA(1, 1, 1, 1);
		}
		g->fillRect(0, 0, size.width, size.height, 5);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "OK");

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(70, 60);
	pop->addObject(img);
	imgSettingBtn[2] = img;

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointMake((devSize.width - 160) / 2, (devSize.height - 160) / 2);
	pop->methodDrawBefore = drawSettingBefore;
	popSetting = pop;

#endif
	setRGBA(1, 1, 1, 1);
	delete g;

}

void freeSettingPopup()
{
	delete popSetting;
	delete imgSettingBtn;
}

void drawSettingPopup(float dt)
{
	popSetting->paint(dt);
}

bool settingDrag = false;
iPoint prevPoint;
bool keySettingPopup(iKeyState stat, iPoint p)
{
	if (popSetting->bShow == false)
		return false;
	if (popSetting->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popSetting->selected;
		if (i == -1) break;

		audioPlay(snd_btn_ok);
		if (i == 0)
		{
			printf("thumb-up\n");
			settingDrag = true;
			prevPoint = p;
		}
		else if (i == 1)
		{
			printf("thumb-down\n");
			settingDrag = true;
			prevPoint = p;
		}
		else// if (i == 2)
		{
			printf("close\n");
			showPopSetting(false);
		}
		break;

	case iKeyStateMoved:
		if (settingDrag)
		{
			iPoint mp = (p - prevPoint);
			prevPoint = p;
			float& x = imgSettingBtn[popSetting->selected]->position.x;
			x += mp.x;
			if (x < rangeBtn[0])		x = rangeBtn[0];
			else if (x > rangeBtn[1])	x = rangeBtn[1];
			float percent = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			printf("%s %.f%%\n",
				popSetting->selected == 0 ? "bgm" : "sfx",
				percent * 100);

			float vol[2];
			for (int i = 0; i < 2; i++)
			{
				float& x = imgSettingBtn[i]->position.x;
				vol[i] = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			}
			audioVolume(vol[0], vol[1], snd_eff_max);
			break;
		}
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgSettingBtn[i]->rect(popSetting->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popSetting->selected)
		{
			printf("audio play btn\n");
			audioPlay(snd_btn_sel);
			popSetting->selected = j;
		}
		break;

	case iKeyStateEnded:
		if (settingDrag)
		{
			settingDrag = false;
			popSetting->selected = -1;
		}
		break;

	}

	return true;
}
void showPopSetting(bool show)
{
	popSetting->show(show);
}
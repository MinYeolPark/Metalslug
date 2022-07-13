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
    iSize size = iSizeMake(120, 60);
    g->init(size);	

	bg = g->createIgImage("assets/UI/Option.png");    
	iPoint p = iPointZero;		
	g->drawIgImage(bg, p.x, p.y, TOP | LEFT);
    Texture* tex = g->getTexture();
    iImage* img = new iImage();
    img->addObject(tex);

	img->position = iPointMake(devSize.width / 2 - bg->GetWidth() / 2, devSize.height / 3);
    freeImage(tex);
	pop->addObject(img);
	//
	// btn (80, 20) 
	//
	const char* str[3] = { "RESUME", "HOME", "QUIT" };
	imgOptionBtn = new iImage * [3];
	size = iSizeMake(90, 10);

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
				setRGBA(1, 0, 1, 1);
			}
			else// if (j == 1)
			{
				setRGBA(0, 1, 0, 1);
			}

			//for (int k = 0; k < strlen(str[i]); k++)
			//{
			//	igImage* ig = normAlphabet[str[i][k] - 'A'];
			//	pos.y = i * (ig->GetHeight() + 3);
			//	g->drawIgImage(ig, pos.x, pos.y, TOP | LEFT);				
			//	pos.x += ig->GetWidth() + 1;
			//	//g->fillRect(0, 0, size.width, size.height, 2);
			//}
			g->fillRect(0, 0, size.width, size.height, 5);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}		

		img->position = iPointMake((devSize.width / 2 - bg->GetWidth() / 2)+ 20, (devSize.height / 3 + 15) + 12 * i);
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
	setRGBA(1, 1, 1, 1);
}

PopupUI::~PopupUI()
{
}

bool PopupUI::paint(float dt)
{	
	popOption->paint(dt);

	bool show = popOption->bShow;
	if (getKeyDown(keyboard_esc))
	{
		show = !show;
		popOption->show(show);
	}
	
    return false;
}

void drawPopOptionBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		popup->imgOptionBtn[i]->frame = (i == popup->popOption->selected);
	printf("i==%d\n", popup->popOption->selected);
}
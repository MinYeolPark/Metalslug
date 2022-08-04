#include "HUDUI.h"

#include "Proc.h"
#include "ProcField.h"
#include "UIMgr.h"

HUDUI* hud;
HUDUI::HUDUI()
{
	setRGBA(1, 1, 1, 1);
	index = 0;

	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(232, 65);
	g->init(size);

	g->fillRect(0, 0, size.width, size.height);
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);

	freeImage(tex);
	pop->addObject(img);


	const char* strMission[2] = { "MISSIONSTART", "MISSIONCLEAR" };
	imgMission = new iImage * [2];
	for (int i = 0; i < 2; i++)
	{
		img = new iImage();
		for (int j = 0; j < strlen(strMission[i]); j++)
		{
			//if (strMission[i][j] == 32)
			//{
			//	g->drawIgImage(redNumber[stage], p.x + 4, p.y, TOP | LEFT);
			//	p.y += 32;
			//	p.x = 0;
			//	continue;
			//}
			igImage* ig = redAlphabet[strMission[i][j] - 'A'];
			g->drawIgImage(ig, p.x, p.y, TOP | LEFT);
			p.x += ig->GetWidth() + 1;
		}
		tex = g->getTexture(iColor4bMake(255, 204, 204, 255));
		img->addObject(tex);
		freeImage(tex);
		
		img->position = iPointZero;
		pop->addObject(img);
		pop->style = iPopupStyleZoom;
		pop->openPoint = iPointMake(0, 0);
		pop->closePoint = iPointMake(0, 0);
		missionPopup = pop;

		missionPopup->show(false);
	}
	
	delete g;

	setRGBA(1, 1, 1, 1);
}

HUDUI::~HUDUI()
{

}

void HUDUI::set(int stage)
{
}

#include "InputMgr.h"
bool HUDUI::paint(float dt)
{	
	if (getKeyDown(keyboard_enter))
	{	
		missionPopup->bShow = !missionPopup->bShow;
		printf(missionPopup->bShow ? "true\n" : "false\n");
	}

	return false;
}

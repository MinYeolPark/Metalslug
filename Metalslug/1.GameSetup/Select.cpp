#include "Select.h"

#include "InputMgr.h"
#include "ImgMgr.h"

#include "Loading.h"
#include "Proc.h"
#include "ProcPlayer.h"

Texture* selectBg;
iImage** imgSelectBtn;
iImage** imgSelectPosBtn;
iImage* imgShutter;
int selectedBtn;
bool confirm = false;
void loadSelect()
{
	selectBg = createImage("assets/CharSelect/CharacterSelectBg.png");
		
	imgSelectBtn = createImgSelect(selectBg);
	return;
#if 1
	imgSelectPosBtn = new iImage * [CharacterIndexMax];
	imgShutter = new iImage();
	for (int j = 0; j < CharacterIndexMax; j++)
	{
		iImage* img = new iImage();
		iImage* img2 = new iImage();
		for (int i = 0; i < 3; i++)
		{
			Texture* t = createImage("assets/CharSelect/CharacterSelect_%d%d.png", i, j);
			img->addObject(t);
			freeImage(t);

			t->width *= devSize.width / selectBg->width;
			t->potWidth *= devSize.width / selectBg->width;
			t->height *= devSize.height / selectBg->height;
			t->potHeight *= devSize.height / selectBg->height;

			t=createImage("assets/CharSelect/P1_%02d.png", j);
			img2->addObject(t);
			freeImage(t);

			t->width *= devSize.width / selectBg->width;
			t->potWidth *= devSize.width / selectBg->width;
			t->height *= devSize.height / selectBg->height;
			t->potHeight *= devSize.height / selectBg->height;
		}		
		img->position = iPointMake( 10 * viewport.size.height / devSize.height + (img->tex->width * j)
			, devSize.height - 65 * viewport.size.height /devSize.height);
		iPoint p[4] = {
			{img->position.x + img->tex->width / 2, img->position.y - 18},
			{img->position.x + img->tex->width / 2 - 6, img->position.y - 18},
			{img->position.x + img->tex->width / 2 + 6, img->position.y - 18},
			{img->position.x + img->tex->width / 2, img->position.y - 18}
		};
		img2->position = p[j];
		img2->anc = HCENTER | VCENTER;
		imgSelectBtn[j] = img;
		imgSelectPosBtn[j] = img2;
	}
	iImage* img = new iImage();
	Texture* t = createImage("assets/CharSelect/Shutter.png");
	img->addObject(t);
	freeImage(t);

	t->width *= devSize.width / selectBg->width;
	t->potWidth *= devSize.width / selectBg->width;
	t->height *= devSize.height / selectBg->height;
	t->potHeight *= devSize.height / selectBg->height;

	imgShutter = img;
	selectedBtn = 0;
#endif
}
void freeSelect()
{
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		delete imgSelectBtn[i];
		delete imgSelectPosBtn[i];
	}
	delete imgSelectBtn;
	delete imgSelectPosBtn;
	delete selectBg;
}
void drawSelect(float dt)
{
	if (getKeyDown(keyboard_right))
	{
		selectedBtn++;
		if (selectedBtn > CharacterIndexMax - 1)
			selectedBtn = 0;
	}
	if (getKeyDown(keyboard_left))
	{
		selectedBtn--;
		if (selectedBtn < 0)
			selectedBtn = CharacterIndexMax - 1;
	}
	if (getKeyDown(keyboard_enter))
	{
		printf("%d", selectedBtn);
		confirm = true;
		imgSelectBtn[selectedBtn]->frame = 2;

		if (confirm = true)
		{
			setLoading(GameStateProc, 2, freeSelect, loadProc);
		}
	}	
	setRGBA(1, 1, 1, 1);
			
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		if (!confirm)
			imgSelectBtn[i]->frame = (i == selectedBtn);			
		imgSelectBtn[i]->paint(dt, iPointZero, iImageTypePop);
	}
	return;
#if 0
	iPoint p[4];
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		p[i]= iPointMake(20 + 78 * i, devSize.height - 180);
		imgShutter->paint(dt, p[i], iImageTypePop);

		iPoint tp = { imgShutter[i].position.x, imgShutter[i].position.y - 5.f };
		if (isLoaded)
			movePoint(imgShutter[i].position, imgShutter[i].position, tp, 5.f);			
	}
	drawImage(selectBg, 0, 0,
		devSize.width / selectBg->width,
		devSize.height / selectBg->height, TOP | LEFT);

	for (int i = 0; i < CharacterIndexMax; i++)
	{
		if (selectedBtn == i)
			imgSelectPosBtn[i]->paint(dt, iPointZero, iImageTypePop);
	}
#endif
	setRGBA(1, 1, 1, 1);
}
bool keySelect(iKeyState stat, iPoint p)
{
#if 1
	int i, j = -1;
	switch (stat) {

	case iKeyStateBegan:
		i = selectedBtn;
		if (i == -1) break;
		if (i == 0)
		{
			confirm = true;
			imgSelectBtn[0]->frame = 2;
		}
		else if (i == 1)
		{
			confirm = true;
			imgSelectBtn[1]->frame = 2;
			printf("eri\n");
		}
		else if (i == 2)
		{
			confirm = true;
			imgSelectBtn[2]->frame = 2;
			printf("tama\n");
		}
		else
		{
			confirm = true;
			imgSelectBtn[3]->frame = 2;
			printf("fio\n");
		}

		if (confirm = true)
			setLoading(GameStateProc, 2, freeSelect, loadProc);
		break;
	case iKeyStateMoved:
		for (i = 0; i < CharacterIndexMax; i++)
		{
			if (containPoint(p, imgSelectBtn[i]->rect()))
			{
				j = i;
				break;
			}
		}
		if (selectedBtn != j)
		{
			;
			selectedBtn = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}
#endif
	return true;
}

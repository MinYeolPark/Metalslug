#pragma once
#include "iStd.h"

enum PopupIndex
{
	PopupOption = 0,
	PopupConfirm,
};
class PopupUI
{
public:
	PopupUI();
	virtual ~PopupUI();

public:
	iPopup* popOption;
	iImage** imgOptionBtn;
public:	
	bool paint(float dt);
	
};
extern int selectedOptionBtn;
extern bool optionConfirm;
extern PopupUI* popup;

void createSettingPopup();
void freeSettingPopup();
void drawSettingPopup(float dt);
bool keySettingPopup(iKeyState stat, iPoint p);
void showPopSetting(bool show);
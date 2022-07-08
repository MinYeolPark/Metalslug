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
	iImage** imgPauseBtn;
public:	
	bool paint(float dt);
	
};

extern PopupUI* popup;

#pragma once

#include "iStd.h"

class HUDUI
{
public:
	HUDUI();
	virtual ~HUDUI();

	int index;			//0: mission start, 1: mission clear
	iPoint p;
public:
	iPopup* missionPopup;
	iImage** imgMission;
	iStrTex* stMission;
	
public:
	void set(int stage);
	bool paint(float dt);
};

extern HUDUI* hud;
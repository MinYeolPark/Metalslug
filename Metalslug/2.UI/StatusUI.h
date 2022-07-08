#pragma once
#include "iStd.h"	

class StatusUI
{
public:
	StatusUI();
	virtual ~StatusUI();

	int num;
	iPoint p;
	float delta;

	igImage** igNumber;
	igImage** alphabet;
	igImage** number;
	igImage** numberGold;

	iStrTex* stPlaytime;

	iStrTex* stScore;
	iStrTex* stAmmo;
	iStrTex* stBomb;
	iStrTex* stLife;

	Texture* statsFrame[3];
	Texture* gaugeFrame[4];

public:
	bool paint(float dt);	

public:

};
extern StatusUI* status;

Texture* methodStPlaytime(const char* str);

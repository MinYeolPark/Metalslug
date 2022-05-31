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

	Texture* statsFrame[3];
	Texture* gaugeFrame[4];

	igImage** alphabet;
	igImage** number;
	igImage** numberGold;

	iStrTex* stScore;
	iStrTex* stAmmo;
	iStrTex* stBomb;
	iStrTex* stLife;

	bool paint(float dt);	
};
extern StatusUI* status;

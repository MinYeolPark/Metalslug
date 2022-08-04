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

	iStrTex* stPlaytime;

	iStrTex* stScore;
	iStrTex* stAmmo;
	iStrTex* stBomb;
	iStrTex* stLife;

	Texture* inf;
	Texture* statsFrame[3];
	Texture* gaugeFrame[4];

public:
	bool paint(float dt);	

public:

};
extern StatusUI* status;

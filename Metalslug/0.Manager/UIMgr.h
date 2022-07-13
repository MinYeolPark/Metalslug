#pragma once

#include "iStd.h"	

extern bool isPopup;

void loadUI();
void freeUI();
void drawUI(float dt, iPoint off);
bool keyProcUI(iKeyState stat, iPoint p);

extern igImage** boldNumber;
extern igImage** normNumber;
extern igImage** goldNumber;
extern igImage** normAlphabet;

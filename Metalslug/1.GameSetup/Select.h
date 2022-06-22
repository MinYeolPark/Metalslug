#pragma once

#include "iStd.h"

extern int selectedBtn;
void loadSelect();
void freeSelect();
void drawSelect(float dt);
bool keySelect(iKeyState stat, iPoint p);
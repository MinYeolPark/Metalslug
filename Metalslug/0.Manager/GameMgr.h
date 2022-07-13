#pragma once

#include "iStd.h"

extern float playtime;
extern bool stageClear;
void loadGame();
void freeGame();
void drawGame(float dt);
bool keyGame(iKeyState stat, iPoint p);

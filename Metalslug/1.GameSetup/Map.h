#pragma once

//추가 업데이트 -> 맵툴
#include "iStd.h"

void loadMap();
void freeMap();
void drawMap(float dt);
bool keyMap(iKeyState stat, iPoint p);

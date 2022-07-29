#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState stat, iPoint p);

// -----------------------------------
// Title Menu Popup
// -----------------------------------
void createTitleMenu();
void freeTitleMenu();
void drawTitleMenu(float dt);
bool keyTitleMenu(iKeyState stat, iPoint p);




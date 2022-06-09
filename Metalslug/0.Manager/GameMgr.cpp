#include "GameMgr.h"

#include "Loading.h"

#include "Menu.h"
#include "Map.h"
#include "Proc.h"
float playtime;
void loadGame()
{
	setTexture(NEAREST, CLAMP);

	playtime = 0.0f;
#if 1
	gs = GameState::GameStateProc;
#else
	gs = GameState::GameStateMenu;
#endif

	loadProc();
	loadMenu();
}

void freeGame()
{
	switch (gs) {
	case GameStateMenu:	freeMenu();		break;
	case GameStateMap:	freeMap();		break;
	case GameStateProc:	freeProc();		break;
	}
}

void drawGame(float dt)
{
	switch (gs) {
	case GameStateMenu:	drawMenu(dt);	break;
	case GameStateMap:	drawMap(dt);	break;
	case GameStateProc:	drawProc(dt);	break;	
	}

	drawLoading(dt);
}

bool keyGame(iKeyState stat, iPoint p)
{
	switch (gs) {
	case GameStateMenu:	keyMenu(stat, p);	break;
	case GameStateMap:	keyMap(stat, p);	break;
	case GameStateProc:	keyProc(stat, p);	break;
	}
	return true;
}
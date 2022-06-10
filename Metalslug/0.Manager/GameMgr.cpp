#include "GameMgr.h"

#include "Loading.h"

#include "Menu.h"
#include "Select.h"
#include "Proc.h"
#include "Map.h"
float playtime;
void loadGame()
{
	setTexture(NEAREST, CLAMP);

	playtime = 0.0f;
#if 0
	gs = GameState::GameStateProc;
#else
	gs = GameState::GameStateMenu;
#endif

	loadMenu();
	//loadProc();
}

void freeGame()
{
	switch (gs) {
	case GameStateMenu:	freeMenu();		break;
	case GameStateSelect: freeSelect();	break;
	case GameStateProc:	freeProc();		break;
	case GameStateMap:	freeMap();		break;
	}
}

void drawGame(float dt)
{
	switch (gs) {
	case GameStateMenu:	drawMenu(dt);	break;
	case GameStateSelect: drawSelect(dt);	break;
	case GameStateProc:	drawProc(dt);	break;	
	case GameStateMap:	drawMap(dt);	break;
	}

	drawLoading(dt);
}

bool keyGame(iKeyState stat, iPoint p)
{
	switch (gs) {
	case GameStateMenu:	keyMenu(stat, p);	break;
	case GameStateSelect: keySelect(stat, p);	break;
	case GameStateProc:	keyProc(stat, p);	break;
	case GameStateMap:	keyMap(stat, p);	break;
	}
	return true;
}
#include "Map.h"

#include "Loading.h"
#include "Menu.h"

void loadMap()
{

}

void freeMap()
{

}

void drawMap(float dt)
{
	setRGBA(1, 0, 0, 1);
	fillRect(0, 0, 50, 50);

	setRGBA(0, 1, 0, 1);
	fillRect(100, 100, 50, 50);

	setRGBA(0, 0, 1, 1);
	fillRect(200, 200, 50, 50);
}

bool keyMap(iKeyState stat, iPoint p)
{
	switch (stat) {
	case iKeyStateBegan:	
		break;
	case iKeyStateMoved:
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}
#include "InputMgr.h"

#include <stdio.h>
static int keyStat = 0;
static int keyDown = 0;
static bool keyUp = false;
static void _setKeyboard(int& keys, bool down, int key)
{
	if (down)
	{
		switch (key) {
		case 'a': case 'A':case 37: keys |= keyboard_left; break;
		case 'd': case 'D':case 39: keys |= keyboard_right; break;
		case 'w': case 'W':case 38: keys |= keyboard_up; break;
		case 's': case 'S':case 40: keys |= keyboard_down; break;
		case 32: keys |= keyboard_space; break;
		case 13: keys |= keyboard_enter; break;
		case 46: keys |= keyboard_delete; break;
		case 16: keys |= keyboard_shift; break;
		case 90: keys |= keyboard_z;	break;
		case 88: keys |= keyboard_x;	break;
		}
		keyUp = false;
	}
	else
	{
		switch (key) {
		case 'a': case 'A':case 37: keys &= ~keyboard_left; break;
		case 'd': case 'D':case 39: keys &= ~keyboard_right; break;
		case 'w': case 'W':case 38: keys &= ~keyboard_up; break;
		case 's': case 'S':case 40: keys &= ~keyboard_down; break;
		case 32: keys &= ~keyboard_space; break;
		case 13: keys &= ~keyboard_enter; break;
		case 46: keys &= ~keyboard_delete; break;
		case 16: keys &= ~keyboard_shift; break;
		case 90: keys &= ~keyboard_z;	break;
		case 88: keys &= ~keyboard_x;	break;
		}
		printf("%d, %d\n", keys, down);
		keyUp = true;
	}
}

void setKeyboard(bool down, int key)
{
	if (down)
	{
		_setKeyboard(keyStat, down, key);
		_setKeyboard(keyDown, down, key);
	}
	else
		_setKeyboard(keyStat, down, key);
}

void updateKeyboard()
{
	keyDown = 0;
}

bool getKeyStat(int key)
{
	return keyStat & key;
}

bool getKeyDown(int key)
{	
	return keyDown & key;
}
bool getKeyUp(int key)
{
	return keyUp && key;
}
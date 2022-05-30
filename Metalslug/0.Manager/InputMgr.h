#pragma once

#define keyboard_none	0
#define keyboard_left	1
#define keyboard_right	2
#define keyboard_up		4
#define keyboard_down	8
#define keyboard_space	16
#define keyboard_enter	32
#define keyboard_delete	64
#define keyboard_shift	128
#define keyboard_z		256
#define keyboard_x		512

void setKeyboard(bool down, int key);
void updateKeyboard();

bool getKeyStat(int key);
bool getKeyDown(int key);
#pragma once

#include "iStd.h"

enum GameState
{
	GameStateMenu = 0,
	GameStateMap,
	GameStateProc,

	GameStateNone
};

extern GameState gs;
typedef void (*MethodLoading)();
void setLoading(GameState gs, MethodLoading free, MethodLoading load);
void drawLoading(float dt);

#define loadingDelta 0.5f

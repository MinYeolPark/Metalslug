#pragma once

#include "iStd.h"

enum GameState
{
	GameStateMenu = 0,
	GameStateSelect,
	GameStateProc,

	GameStateMap,

	GameStateNone
};

extern bool isLoaded;
extern GameState gs;
typedef void (*MethodLoading)();
void setLoading(GameState gs, MethodLoading free, MethodLoading load);
void setLoading(GameState gs, float delayTime, MethodLoading free, MethodLoading load);
void drawLoading(float dt);

#define loadingDelta 0.5f

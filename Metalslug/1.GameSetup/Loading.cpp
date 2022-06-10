#include "Loading.h"

GameState gs;

bool isLoaded = false;
static float delay, _delay;
static GameState gsNew;
static MethodLoading methodFree, methodLoad;
static float loadingDt = 0.0f;
void setLoading(GameState gs, MethodLoading free, MethodLoading load)
{
	if (loadingDt)
		return;

	_delay = 0.f;
	delay = 0.f;
	gsNew = gs;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.000001f;
}

void setLoading(GameState gs, float delayTime, MethodLoading free, MethodLoading load)
{
	if (loadingDt)
		return;
	
	_delay = delayTime;
	delay = 0.f;
	gsNew = gs;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.000001f;
}

void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;
	else
		delay += dt;

	if (delay < _delay)
		return;

	float alpha = 1.0f;

	// 어두워짐
	if (loadingDt < loadingDelta)
	{
		// 어두워지고 있음.
		alpha = loadingDt / loadingDelta;

		loadingDt += dt;
		if (loadingDt >= loadingDelta)
		{
			// 완전 어두워짐
			alpha = 1.0f;

			methodFree();
			gs = GameStateNone;
			loadingDt = loadingDelta;
		}
	}
	else if (loadingDt < loadingDelta+0.7f)
	{
		// 완전 어두워짐
		alpha = 1.0f;

		loadingDt += dt;
		if (loadingDt >= loadingDelta + 0.7f)
		{
			methodLoad();
			gs = gsNew;
			loadingDt = loadingDelta + 0.7f;
		}
	}
	else// if (loadingDt < loadingDelta * 2 + 0.7f)
	{
		// 점점 밝아짐
		alpha = 1.0f - (loadingDt - loadingDelta - 0.7f) / loadingDelta;

		loadingDt += dt;
		if (loadingDt > loadingDelta * 2 + 0.7f)
		{
			loadingDt = 0.0f;// 완전 밝아짐
			isLoaded = true;
		}
	}

	// alpha 블랙 그림
	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
}

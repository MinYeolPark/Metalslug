#include "GameMgr.h"

#include "Loading.h"

#include "UIMgr.h"
#include "Menu.h"
#include "Select.h"
#include "Proc.h"
#include "Map.h"
float playtime;
bool stageClear;
AudioInfo ai[];
void loadGame()
{
	setTexture(NEAREST, CLAMP);

	playtime = 0.0f;
	stageClear = false;

	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);

#if 0
	gs = GameState::GameStateProc;
	loadProc();
#else
	gs = GameState::GameStateMenu;
	loadMenu();
#endif
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

static float endDt, _endDt = 3.f;
void drawGame(float dt)
{
	switch (gs) {
	case GameStateMenu:	drawMenu(dt);	break;
	case GameStateSelect: drawSelect(dt);	break;
	case GameStateProc:	drawProc(dt);	break;	
	case GameStateMap:	drawMap(dt);	break;
	}
	drawLoading(dt);

	if (stageClear)
	{
		endDt += dt;
		if (endDt > _endDt)
		{
			//Game End						
			audioPlay(snd_bgm_clear);
			audioStop(snd_bgm_boss);
			showMissionPopup(true, 0.000001f);
			if (stageClear)
				setLoading(GameStateMenu, 6, freeProc, loadMenu);
			stageClear = false;
		}
	}
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


AudioInfo ai[] = {
	{ (char*)"assets/sound/pop_close.wav", false, 1.0f },
	{ (char*)"assets/sound/pop_open.wav", false, 1.0f },
	{ (char*)"assets/sound/nar_m1str.wav", false, 1.0f },
	{ (char*)"assets/sound/select_eri.wav", false, 1.0f },
	{ (char*)"assets/sound/nar_HMG.wav", false, 1.0f },
	{ (char*)"assets/sound/ok.wav", false, 1.0f },	
	{ (char*)"assets/sound/fire.wav", false, 0.8f },
	{ (char*)"assets/sound/melee.wav", false, 0.8f },
	{ (char*)"assets/sound/HMG_fire.wav", false, 0.8f },
	{ (char*)"assets/sound/reload.wav", false, 1.0f },
	{ (char*)"assets/sound/eri_dead.wav", false, 1.0f },
	{ (char*)"assets/sound/scream2.wav", false, 1.0f },
	{ (char*)"assets/sound/thankyou.wav", false, 1.0f },
	{ (char*)"assets/sound/eff_bomb.wav", false, 1.0f },
	{ (char*)"assets/sound/ExplosionM.wav", false, 1.0f },
	{ (char*)"assets/sound/ExplosionL.wav", false, 1.0f },
	{ (char*)"assets/sound/mosqueMove.wav", false, 1.0f },
	{ (char*)"assets/sound/title.wav", false, 1.0f },
	{ (char*)"assets/sound/bgm_select.wav", true, 1.0f },
	{ (char*)"assets/sound/bgm_stage1.wav", true, 1.0f },
	{ (char*)"assets/sound/bgm_boss.wav", true, 1.0f },
	{ (char*)"assets/sound/bgm_clear.wav", true, 1.0f },
};
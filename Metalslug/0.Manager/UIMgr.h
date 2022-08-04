#pragma once

#include "iStd.h"	

extern bool isPopup;

void loadUI();
void freeUI();
void drawUI(float dt, iPoint off);
bool keyProcUI(iKeyState stat, iPoint p);

void createConfirmPopup();
void freeConfirmPopup();
void drawConfirmPopup(float dt);
bool keyConfirmPopup(iKeyState stat, iPoint p);

extern iPopup* countDownPopup;
extern int countDown;
void createCountdownPopup();
void freeCountdownPopup();
void drawCountdownPopup(float dt);
void showPopCountdown(bool show);

void createMissionPopup();
void freeMissionPopup();
void drawMissionPopup(float dt);
void showMissionPopup(bool show);
void showMissionPopup(bool show, float delta);

extern igImage** boldNumber;
extern igImage** normNumber;
extern igImage** goldNumber;
extern igImage** bigNumber;
extern igImage** redNumber;

extern igImage** normAlphabet;
extern igImage** redAlphabet;
enum snd
{
	snd_btn_sel = 0,
	snd_btn_ok,

	snd_nar_m1_start,
	snd_nar_select_eri,
	snd_nar_HMG,
	snd_nar_OK,

	snd_nar_max,	

	snd_eff_fire = snd_nar_max,
	snd_eff_melee,
	snd_eff_HMG_fire,
	snd_eff_reload,
	snd_eff_eridead,
	snd_eff_enemydead,
	snd_eff_thank,

	snd_eff_bomb,
	snd_eff_expM,
	snd_eff_expL,

	//Enemies
	snd_eff_mosqueMove,

	snd_eff_max,

	snd_bgm_title = snd_eff_max,
	snd_bgm_select,
	snd_bgm_stage1,
	snd_bgm_boss,
	snd_bgm_clear,

	snd_max,
};
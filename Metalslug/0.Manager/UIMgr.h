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

extern igImage** boldNumber;
extern igImage** normNumber;
extern igImage** goldNumber;
extern igImage** normAlphabet;

enum snd
{
	snd_btn_sel = 0,
	snd_btn_ok,

	snd_m1_start,
	snd_nar_max,

	snd_eff_fire = snd_nar_max,
	snd_eff_select_eri,
	snd_eff_max,

	snd_bgm_title = snd_eff_max,
	snd_bgm_select,
	snd_bgm_stage1,

	snd_max,
};
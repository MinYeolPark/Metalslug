#include "UIMgr.h"

#include "GameMgr.h"
#include "StatusUI.h"

iStrTex* stPlaytime;
void loadUI()
{
	status = new StatusUI();
}

void freeUI()
{

}

void drawUI(float dt, iPoint off)
{
	status->paint(dt);

	
}

void addUI(iPoint p, int num)
{

}

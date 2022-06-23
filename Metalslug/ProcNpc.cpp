#include "ProcNpc.h"

#include "Ichimondi.h"
#include "ProcItem.h"
ProcNpc*** _npc;
ProcNpc** npc;
int npcNum;
void loadProcNpcs()
{
	_npc = new ProcNpc * * [NpcIndexMax];
	for (int i = 0; i < NpcIndexMax; i++)
	{
		_npc[i] = new ProcNpc * [npcMax];
		if (i == NpcIchimonji)
			for (int j = 0; j < npcMax; j++)
				_npc[i][j] = new Ichimondi(i);
	}
	npc = new ProcNpc * [NpcIndexMax * npcMax];
	npcNum = 0;

	//AddProcNpc
}

void freeProcNpcs()
{
	for (int i = 0; i < NpcIndexMax; i++)
	{
		delete _npc[i];
		delete npc[i];
	}
	delete _npc;
	delete npc;
}

void drawProcNpcs(float dt, iPoint off)
{
	for (int i = 0; i < npcNum; i++)
	{
		ProcNpc* n = npc[i];
		n->update(dt);
		if (n->draw(dt, off))
		{
			npcNum--;
			npc[i] = npc[npcNum];
			i--;
		}
	}
}

void addNPC(int index, iPoint p)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npc[index][i];
		if (n->isActive == false)
		{
			n->set(index, p);
			npc[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

void addNPC(int index, iPoint p, int itemIndex)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npc[index][i];
		if (n->isActive == false)
		{
			n->set(index, p, itemIndex);
			npc[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

ProcNpc::ProcNpc()
{
	p = iPointZero;
	v = iPointZero;
	s= iSizeZero;
	index = (NpcIndex)0;
	state = (NpcBehave)0;
	isActive=false;
	complete = false;
	itemIndex = 0;
	up = 0.f;
	down = 0.f;
	fall = false;
	moveSpeed = 100.f;
}

ProcNpc::~ProcNpc()
{

}

void ProcNpc::cbAniSpawnItem(void* n)
{
	ProcNpc* npc = (ProcNpc*)n;	
	
	npc->complete = true;
	npc->spawnItem();		
	npc->setState((NpcBehave)(SaluteNpcL + npc->state % 2));
}

void ProcNpc::cbAniSalute(void* n)
{
	ProcNpc* npc = (ProcNpc*)n;

	npc->setState((NpcBehave)(EscapeNpcL + npc->state % 2));
	npc->v = iPointRotate(iPointMake(1, 0), iPointZero, 0);
}

void ProcNpc::cbAniNpcOut(void* n)
{
}


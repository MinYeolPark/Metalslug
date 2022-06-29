#include "ProcNpc.h"

#include "Ichimondi.h"

#include "ProcItem.h"
#include "ProcPlayer.h"
ProcNpc*** _npc;
ProcNpc** npc;
int npcNum;
void loadProcNpcs()
{
	_npc = new ProcNpc * * [NpcIndexMax];
	for (int i = 0; i < NpcIndexMax; i++)
	{
		_npc[i] = new ProcNpc * [npcMax];
		if (i == NpcIchimondi)
			for (int j = 0; j < npcMax; j++)
				_npc[i][j] = new Ichimondi(i);
	}
	npc = new ProcNpc * [NpcIndexMax * npcMax];
	npcNum = 0;

	//AddProcNpc
	//addProcNPC(NpcIchimondi, iPointMake(150, 100));
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

void addProcNPC(int index, iPoint p)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npc[index][i];
		if (n->isActive == false)
		{
			int r = rand() % ItemIndexMax;
			n->init(index, p, r);
			npc[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

void addProcNPC(int index, iPoint p, int itemIndex)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npc[index][i];
		if (n->isActive == false)
		{
			n->init(index, p, itemIndex);
			npc[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

ProcNpc::ProcNpc(int index)
{
}

ProcNpc::~ProcNpc()
{

}

void ProcNpc::getDamage(float damage, Collider* c)
{
	if (getState() == (NpcBehave)(IdleNpcL + state % 2))
	{
		setState((NpcBehave)(ReleaseNpcL + state % 2));		
		for(int i=0;i<1;i++)
			colliders[i]->disable();
	}
}

void ProcNpc::init(int idex, iPoint p, int itemIndex)
{
	isActive = true;
	this->itemIndex = itemIndex;
	this->p = p;
	this->index = (NpcIndex)index;

	objects->addObject(this);
}

void ProcNpc::fixedUpdate(float dt)
{
	//for(int i=0;i<1;i++)
	//	colliders[i]->setPosition(p);
	int maxY = *(map->maxY + (int)p.x);
	if (p.y >= maxY)
	{
		up = 0;
		down = 0;
		fall = false;

		p.y = maxY;
	}
	else
		fall = true;

	if (fall)
	{
		if (p.y < maxY)
		{
			down += jumpDecrease * dt;
			p = (iPointMake(p.x, p.y += down));
		}
	}
}

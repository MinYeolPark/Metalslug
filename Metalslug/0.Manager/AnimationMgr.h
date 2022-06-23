#pragma once
class AnimationMgr
{
public:
	static void cbAniDead(void* parm);
	static void cbAniAttack(void* parm);

public:
	//Enemies
	static void cbAniMeleeFire(void* parm);

public:
	//Npcs
	static void cbAniNpcRelease(void* parm);
	static void cbAniNpcSpawnItem(void* n);
	static void cbAniNpcSalute(void* n);
};


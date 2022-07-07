#pragma once
class AnimationMgr
{
public:
	static void cbAniToIdle(void* parm);
	static void cbAniToCrouch(void* parm);
	static void cbAniDead(void* parm);
	static void cbAniAttack(void* parm);

public:
	//Enemies
	static void cbAniEnemyMotion2Idle(void* parm);
	static void cbAniMeleeFire(void* parm);
	static void cbAniTruck(void* parm);

public:
	static void cbAniMosque(void* parm);

public:
	//Npcs
	static void cbAniNpcRelease(void* parm);
	static void cbAniNpcSpawnItem(void* n);
	static void cbAniNpcSalute(void* n);

public:
	//Bullests
	static void cbAniBulletDisappear(void* parm);
	static void cbAniBulletDisappearWithAlpha(void* parm);
};


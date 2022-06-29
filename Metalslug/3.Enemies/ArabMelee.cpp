#include "ArabMelee.h"

#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgMeleeInfo[];
static iImage** _imgMelee = NULL;
ArabMelee::ArabMelee(int index) : ProcEnemy(index)
{	
	this->index = index;
	state = IdleMeleeL;	
	EnemyAI ai[2] =
	{
		ProcEnemyAI::ArabMeleeAI0,
		ProcEnemyAI::ArabMeleeKessie,
	};
	this->ai = ai[index];

	hp = 100;
	dmg = 100;
	sight = 200;
	moveSpeed = 150.f;
	attkRange = 70;
	attkRate = 0.f;	_attkRate = 2.f;
	aiDt = 0.f;	_aiDt = 2.f;
#if 1
	colNum = 1;
	for (int i = 0; i < colNum; i++)
		colliders[i]->init(this, iSizeMake(40, 40));
#endif
	imgs = NULL;
	imgCurr = NULL;	

	up = 0.f;
	down = 0.f;
	fall = false;

	if( _imgMelee==NULL )
		_imgMelee = createImgReverse(imgMeleeInfo, MeleeBehaveMax, this);

	imgs = new iImage * [MeleeBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * MeleeBehaveMax);
	for (int i = 0; i < MeleeBehaveMax; i++)
		imgs[i] = _imgMelee[i]->clone();
	imgCurr = imgs[index];
}

ArabMelee::~ArabMelee()
{	
	for (int i = 0; i < MeleeBehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

int ArabMelee::getFrame()
{
	printf("imgs[state]->frame= %d\n", imgs[state]->frame);
	return imgs[state]->frame;
}

bool ArabMelee::dead()
{
	isDead = true;
	for (int i = 0; i < colNum; i++)
	{
		colliders[i]->disable();
		//objects->removeObject(colliders[i]);
	}
	state = (DeadMeleeL + state % 2);
	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);

	return state == (DeadMeleeL + state % 2);
}

void ArabMelee::getDamage(float damage, Collider* c)
{
	hp -= damage;
	if (hp <= 0)
	{
		if (!isDead)
			dead();
	}
}

void ArabMelee::setState(int newState)
{
	state = newState;

	if (state == FireMeleeL || state == FireMeleeR)
	{
		imgs[state]->startAnimation(AnimationMgr::cbAniMeleeFire, this);
	}
}

int len;
void ArabMelee::update(float dt)
{
	isActive = containPoint(p,
		iRectMake(-map->off.x - 40, -map->off.y - 40,
			devSize.width + 80, devSize.height + 80));
	fp = { p.x, p.y - 20 };
	if (v != iPointZero)
	{
		if (v.x > 0)
			setState(WalkMeleeR);
		else if (v.x < 0)
			setState(WalkMeleeL);
	}

	len = iPointLength(player->p - p);
	if (!isAppear)
	{
		float mx = p.x + map->off.x;
		float ip = devSize.width * 3 / 4;
		if (p.x - ip > 0)
			v.x = -1;
		else if (p.x + ip < 0)
			v.x = 1;
		if (mx > ip)
		{
			if (movePoint(p, p, iPointMake(ip, p.y), moveSpeed * dt))
			{
				v = iPointZero;
				isAppear = true;
				setState(PreAttackMeleeL + state % 2);
			}
		}
	}
	else
	{
		aiDt += dt;
		if (aiDt > _aiDt)
		{
			aiDt -= _aiDt;
#if 0
			EnemyAI ai[2] =
			{
				ProcEnemyAI::ArabMeleeAI0,
				ProcEnemyAI::ArabMeleeAI1,
			};
			int r = rand() % 2;
			this->ai = ai[r];
#endif
			this->ai(this, dt);
		}
	}
	
	p.y = *(map->maxY + (int)p.x);
	fixedUpdate(dt);
}
int bNum = 0;
bool ArabMelee::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

	if (state == FireMeleeL)
	{
		if (imgCurr->frame == 6)
		{ 
			if (bNum == 0)
			{
				addBullet(this, BulletMelee, 180);
				bNum++;
			}
		}
	}	
#ifdef _DEBUG
	drawDot(p + off);
	for (int i = 0; i < colNum; i++)
		drawRect(colliders[i]->getCollider());
	setRGBA(1, 1, 1, 0.5);
	setLineWidth(10);
	drawLine(p, { p.x - len,p.y });
	setStringSize(20);
	setStringName("assets/BMJUA_ttf.ttf");
	setStringRGBA(1,0,0,1);
	drawString(p.x - 20, p.y - 20, TOP|LEFT, "Len : %3d", len);
	setLineWidth(2);
#endif // DEBUG
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ArabMelee::free()
{
	//#issue한번만 지우기
	if (_imgMelee != NULL)
	{
		for (int i = 0; i < MeleeBehaveMax; i++)
			delete _imgMelee[i];
		delete _imgMelee;
		_imgMelee = NULL;
	}
}


ImageInfo imgMeleeInfo[] =
{
	{
		"assets/ArabMelee/ArabMelee_Idle_%02d.png",
		9, 1.0f, { -36, 0},
		0.18f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_Move_%02d.png",
		12, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/Arab_Melee_Dead_%02d.png",
		20, 1.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniDead,
	},
	{
		"assets/ArabMelee/ArabMelee_Shuffle_%02d.png",
		6, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_AttackMelee_%02d.png",
		8, 1.0f, { -36, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_AttackPre_%02d.png",
		4, 1.0f, { -36, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_Fire_%02d.png",
		19, 1.0f, { -36, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniMeleeFire,
	},
	{
		"assets/ArabMelee/ArabMelee_Jump_%02d.png",
		9, 1.0f, { -36, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniEnemyMotion2Idle,
	},
};

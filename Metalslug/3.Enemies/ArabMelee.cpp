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
	moveSpeed = 80.f;
	attkRange = 120;
	attkRate = 0.f;	_attkRate = 1.f;
	aiDt = 0.f;	_aiDt = 5.f;
	reload = 1;
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

void ArabMelee::update(float dt)
{
	if (isDead)
	{		
		return;
	}	

	if (!isAppear)
	{
		if (containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80)))
			isAppear = true;
	}
	else
	{
		isActive = containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80));
	}

	fp = { p.x, p.y - 20 };
	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		EnemyAI ai[2] = {
			ProcEnemyAI::ArabMeleeAI0,
			ProcEnemyAI::ArabMeleeAI1,
		};
		int r = rand() % 2;
		this->ai = ai[r];

		//test
		this->ai = ai[0];
		this->ai(this, dt);			//update tp -> pattern
	}

	if (p != tp)
	{
		if (p.x < tp.x)
		{
			p.x += v.x;
			if (p.x > tp.x)
				p.x = tp.x;
		}
		else if (p.x > tp.x)
		{
			p.x += v.x;
			if (p.x < tp.x)
				p.x = tp.x;
		}

		if (p.y < tp.y)
		{
			p.y += v.y;
			if (p.y > tp.y)
				p.y = tp.y;
		}
		else if (p.y > tp.y)
		{
			p.y += v.y;
			if (p.y < tp.y)
				p.y = tp.y;
		}
	
		if (p == tp)
			v = iPointZero;
	}

	//Animation
	if (v != iPointZero)
	{
		if (v.x > 0)
		{
			setState(WalkMeleeR);
			isRight = true;
		}
		else if (v.x < 0)
		{
			setState(WalkMeleeL);
			isRight = false;
		}
	}
	else
	{
		int len = iPointLength(p - player->p);
		if (len < sight)
		{
			attkRate += dt;
			if (state != (ArabMeleeBehave)PreAttackMeleeL + state % 2
				&& state != (ArabMeleeBehave)FireMeleeL + state % 2)
			{
				setState((ArabMeleeBehave)PreAttackMeleeL + state % 2);
			}
			if (attkRate > _attkRate)
			{
				attkRate -= _attkRate;
				reload = 1;
				if (state != (ArabMeleeBehave)FireMeleeL + state % 2)
					setState((ArabMeleeBehave)FireMeleeL + state % 2);								
			}
		}

		//Except
		if (player->isDead)
		{
			if (state != (ArabMeleeBehave)IdleMeleeL + state % 2)
				setState((ArabMeleeBehave)IdleMeleeL + state % 2);
		}
	}
	p.y = *(map->maxY + (int)p.x);

	fixedUpdate(dt);
}
bool ArabMelee::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

	if (state == (ArabMeleeBehave)FireMeleeL + state % 2)
	{
		if (imgCurr->frame == 6)
		{ 
			if (reload)
			{				
				reload -= 1;
				addBullet(this, BulletMelee, 0);
			}
		}
	}
#ifdef _DEBUG
	drawDot(p + off);
	iRect c = colliders[0]->getCollider();	
	c.origin.x += off.x;
	c.origin.y += off.y;
	drawRect(c);
	setRGBA(1, 0, 1, 0.5);

	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(20);
	int len = iPointLength(tp - p);
	drawString(p.x, p.y, TOP | LEFT, "%d", len);
	
	setLineWidth(10);	
	drawLine(p + off, tp + off);
	setLineWidth(1);
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

int ArabMelee::getFrame()
{
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
		imgs[state]->startAnimation(AnimationMgr::cbAniEnemyMotion2Idle, this);
	}
	if (state == AttackMeleeL || state == AttackMeleeR)
	{
		imgs[state]->startAnimation(AnimationMgr::cbAniEnemyMotion2Idle, this);
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
		NULL,
	},
	{
		"assets/ArabMelee/ArabMelee_Jump_%02d.png",
		9, 1.0f, { -36, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		NULL,
	},
};

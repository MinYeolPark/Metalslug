#include "Truck.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"

#include "Abul.h"
ImageInfo imgTruckInfo[];
static iImage** _imgTruck = NULL;
Truck::Truck(int index) : ProcEnemy(index)
{
    score = 100;
    this->index = index;
    state = IdleTruckL;

    hp = 2000;
    dmg = 0;
    sight = 200;
    moveSpeed = 80;
    attkRange = 0;
    attkRate = 0.f; _attkRate = 0.f;
    aiDt = 0.f; _aiDt = 2.f;
    dmgDt = 0.f; _dmgDt = 0.18f;
    imgs = NULL;
    imgCurr = NULL;

    up = 0.f;
    down = 0.f;
    fall = false;

    if (_imgTruck == NULL)
        _imgTruck = createImgReverse(imgTruckInfo, TruckBehaveMax, this);

    imgs = new iImage * [TruckBehaveMax];
    memset(imgs, 0x00, sizeof(iImage*) * TruckBehaveMax);
    for (int i = 0; i < TruckBehaveMax; i++)
        imgs[i] = _imgTruck[i]->clone();
    imgCurr = imgs[index];

    rectNum = 1;
    rect = new iRect * [rectNum];
    for (int i = 0; i < rectNum; i++)
        rect[i] = new iRect();
}

Truck::~Truck()
{
    for (int i = 0; i < TruckBehaveMax; i++)
        delete imgs[i];
    delete imgs;
}

void Truck::init(iPoint p)
{
    this->isActive = true;
    this->index = index;
    this->p = p;
    this->tp = p;

    for (int i = 0; i < rectNum; i++)
    {
        iRect* r = rect[i];
        r->size = iSizeMake(40, 40);
        r->origin = p;
    }
}

void Truck::update(float dt)
{
    if (isDead)
    {
        for (int i = 0; i < rectNum; i++)
            rect[i]->size = iSizeZero;
        return;
    }

    if (!isAppear)
    {
        if (player->p.x > 1550)
        {            
            state= (MoveTruckL + state % 2);            
            if (movePoint(p, p, iPointMake(1650, p.y), moveSpeed * dt))
            {
                addProcEnemy(IdxAbul, iPointMake(1740, p.y));
                state = DeployTruckL + state % 2;                
                _aiDt = 3.0f;
                isAppear = true;
            }
        }
    }
    else
    {
        isActive = containPoint(p,
            iRectMake(-map->off.x - 40, -map->off.y - 40,
                devSize.width + 80, devSize.height + 80));

        aiDt += dt;
        if (aiDt > _aiDt)
        {
            aiDt -= _aiDt;
            //addProcEnemy(IdxBurserker, { p.x - 20 , p.y });
        }
    }

    if (v != iPointZero)
    {
        v /= iPointLength(v);
        v *= (moveSpeed * dt);
        //move reverse
        if (v.x < 0)
            state = MoveTruckL;            
        if (v.x > 0)
            state = MoveTruckR;            
    }    
    if (player->isDead)
        aiDt = 0.0f;
    fixedUpdate(dt);
}

void Truck::fixedUpdate(float dt)
{
    ProcEnemy::fixedUpdate(dt);

    //ColliderUpdate
    for (int i = 0; i < rectNum; i++)
    {
        rect[i]->origin = iPointMake(
            p.x + map->off.x - rect[i]->size.width / 2,
            p.y + map->off.y - rect[i]->size.height);
    }
}

bool Truck::draw(float dt, iPoint off)
{
    if (dmgDt)
    {
        setRGBA(1, 0.8, 0.1, 1);
        dmgDt += dt;
        if (dmgDt > _dmgDt)
            dmgDt = 0.f;
    }
    else
        setRGBA(1, 1, 1, 1);

    imgCurr = imgs[state];
    imgCurr->paint(dt, p + off);

#ifdef DEBUG
    for (int i = 0; i < rectNum; i++)
        drawRect(getRect());

#endif // _DEBUG
    setRGBA(1, 1, 1, 1);
    return !isActive;
}

void Truck::free()
{
   
}

void Truck::getDamage(float damage)
{
    hp -= damage;
    player->addScore(score);
    if (hp <= 0)
    {
        if (!isDead)
        {
            isDead = true;
            state = (DeadTruckL + state % 2);
            imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
            addProcEffect(this, EffectExplosionL, p);
            for (int i = 0; i < enemyNum; i++)
            {
                ProcEnemy* e = enemies[i];
                if (e->index == IdxAbul)
                    e->tp = iPointMake(e->p.x + 400, e->p.y);
            }
        }
    }
}

ImageInfo imgTruckInfo[] =
{
    {
        "assets/Truck/Truck_Idle.png",
        1, 1.0f, { -1119 / 2, 0},
        0.1f,
        0,			//repeat
        {255, 0, 0, 255},
        NULL,
    },
    {
        "assets/Truck/Truck_Move_%02d.png",
        4, 1.0f, { -121 / 2, 0},
        0.1f,
        0,			//repeat
        {255, 0, 0, 255},
        NULL,
    },
    {
        "assets/Truck/Truck_Deploying_%02d.png",
        11, 1.0f, { -141 / 2, 0},
        0.1f,
        1,			//repeat
        {255, 255, 255, 255},
        AnimationMgr::cbAniTruck,
    },
    {
        "assets/Truck/Truck_Dead_%02d.png",
        6, 1.0f, { -1119 / 2, 0},
        0.1f,
        0,			//repeat
        {255, 0, 0, 255},
        NULL,
    },
};
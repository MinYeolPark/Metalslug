#include "Truck.h"

#include "EnemyMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgTruckInfo[];
static iImage** _imgTruck = NULL;
Truck::Truck(int index) : ProcEnemy(index)
{
    this->index = index;
    state = IdleTruckL;
    ai = ProcEnemyAI::TruckAI0;

    hp = 2000;
    dmg = 0;
    sight = 300;
    moveSpeed = 80;
    attkRange = 0;
    attkRate = 0.f; _attkRate = 0.f;
    aiDt = 0.f; _aiDt = 0.f;

    colNum = 1;
    for (int i = 0; i < colNum; i++)
        colliders[i]->init(this, iSizeMake(100, 70));

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
}

Truck::~Truck()
{
    for (int i = 0; i < TruckBehaveMax; i++)
        delete imgs[i];
    delete imgs;
}

int Truck::getFrame()
{
    return 0;
}

bool Truck::dead()
{
    isDead = true;
    for (int i = 0; i < colNum; i++)
    {
        colliders[i]->disable();
        //objects->removeObject(colliders[i]);
    }
    state = (DeadTruckL + state % 2);
    addProcEffect(EffectExplosion, p);
    imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);

    return state == (DeadTruckL + state % 2);
}

void Truck::getDamage(float damage, Collider* c)
{
    hp -= damage;
    if (hp <= 0)
    {
        if (!isDead)
            dead();
    }
}

void Truck::setState(int newState)
{
    state = newState;

    if (state == DeployTruckL || state == DeployTruckR)
    {
        v = iPointZero;
    }
}

void Truck::update(float dt)
{
    isActive = containPoint(p,
        iRectMake(-map->off.x, -map->off.y - 20,
            devSize.width + 400, devSize.height + 40));
    if (v != iPointZero)
    {
        v /= iPointLength(v);
        v *= (moveSpeed * dt);
        //move reverse
        if (v.x < 0)
            setState(MoveTruckL);
        if (v.x > 0)
            setState(MoveTruckR);
        if (movePoint(p, p, iPointMake(1600, p.y), moveSpeed * dt))
        {
            setState(DeployTruckL + state % 2);   
            _aiDt = 3.0f;
            isAppear = true;
        }
    }
    if (isAppear)
    {
        aiDt += dt;
        if (aiDt > _aiDt)
        {
            aiDt -= _aiDt;
            addProcEnemy(IdxArBurserker, { p.x - 20, p.y }, iPointZero);
        }
    }    
    fixedUpdate(dt);
}

bool Truck::draw(float dt, iPoint off)
{
    setRGBA(1, 1, 1, 1);
    imgCurr = imgs[state];
    imgCurr->paint(dt, p + off);

#ifdef _DEBUG
    drawDot(p + off);
    for (int i = 0; i < colNum; i++)
        drawRect(colliders[i]->getCollider());
#endif // DEBUG
    setRGBA(1, 1, 1, 1);
    return !IsAccelerator;
}

void Truck::free()
{
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
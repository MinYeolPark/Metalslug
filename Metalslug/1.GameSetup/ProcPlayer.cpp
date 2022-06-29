#include "ProcPlayer.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "BulletMgr.h"
#include "InputMgr.h"

#include "Proc.h"
#include "ProcField.h"
#include "ProcItem.h"
ProcPlayer* player;
static iImage** _imgEriHeavyTop = NULL;
ImageInfo topImageInfo[];
ImageInfo botImageInfo[];
ImageInfo infoEriTopHeavy[];
static iImage** _imgEriTop = NULL;
static iImage** _imgEriBot = NULL;
ProcPlayer::ProcPlayer(int index) : ProcObject()
{
    layer = ObjLayer::LayerPlayer;
    p = iPointZero;
    v = iPointZero;
    s = iSizeZero;

    isActive = false;
    this->index = index;
    hp = 100, _hp = 100;
    score = 0;

#if 1
    colNum = 1;
    for (int i = 0; i < colNum; i++)
        colliders[i]->init(this, iSizeMake(40, 40));
#endif
    topImgs = NULL;
    topImgCurr = NULL;
    botImgs = NULL;
    botImgCurr = NULL;

    curGun = new Gun();//{ HandGun, 100, 100, 0 };
    firePoint = iPointMake(p.x + 10, p.y - 15);
    bombPoint = p;
    up = 0;
    down = 0;
    fall = true;

    isDead = false;
    fireing = false;
    dirUp = false;

    life = 3;
    moveSpeed = 150.f;
    bombSpeed = 10.f;
    attkRange = 40;
    bombRange = 100.0f;
    bombs = 20;

#if 1
    alpha = 1.0f;
    alphaDt = 0.0f;
#endif
    topState = PlayerIdle;
    botState = PlayerIdle;

    _imgEriTop = createSingleImage(topImageInfo, PlayerBehaveMax, this);
    _imgEriHeavyTop = createSingleImage(infoEriTopHeavy, PlayerBehaveMax, this);
    _imgEriBot = createSingleImage(botImageInfo, 8, this);
    switch (index)
    {
    case MARCO:   printf("No imgs");                            break;
    case ERI:     topImgs = _imgEriTop; botImgs = _imgEriBot;   break;
    case TAMA:    printf("No imgs");                            break;
    case PIO:     printf("No imgs");                            break;
    }
}

ProcPlayer::~ProcPlayer()
{
    for (int i = 0; i < 16; i++)
    {
        delete _imgEriTop[i];
        delete _imgEriHeavyTop[i];
    }
    for (int i = 0; i < 4; i++)
        delete _imgEriBot[i];
    delete _imgEriTop;
    delete _imgEriHeavyTop;
    delete _imgEriBot;

    delete curGun;
    delete player;

    _imgEriTop = NULL;
    _imgEriHeavyTop = NULL;
    _imgEriBot = NULL;
}

void ProcPlayer::init(iPoint p)
{
    isActive = true;
    isDead = false;
    topState = PlayerSpawn;
    botState = PlayerIdle;
    this->p = p;
    curGun->gunIndex = Handgun;
    alpha = 1.0f;
    bombs = 10;
    life--;

    if (topState == PlayerSpawn)
        topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);

    for(int i=0;i<colNum;i++)
        objects->addObject(colliders[i]);
}

void ProcPlayer::update(float dt)
{
    v = iPointZero;
    if (getKeyStat(keyboard_left))
        v.x = -1;
    else if (getKeyStat(keyboard_right))
        v.x = 1;
    if (getKeyStat(keyboard_up))
        v.y = -1;
    else if (getKeyStat(keyboard_down))
        v.y = 1;

    if (v != iPointZero)
    {
        v.setLength(moveSpeed * dt);
        if (v.x < 0)
        {
            dirRight = false;
            fireDeg = 180;
            firePoint = iPointMake(p.x - 10, p.y - 10);
        }
        else if (v.x > 0)
        {
            dirRight = true;
            fireDeg = 0;
            firePoint = iPointMake(p.x + 10, p.y - 10);
        }

        if (v.y < 0)
        {
            dirUp = false;
            fireDeg = 270;
            if (dirRight)
                firePoint = iPointMake(p.x + 5, p.y - 40);
            else
                firePoint = iPointMake(p.x - 5, p.y - 40);
        }
        else if (v.y > 0)
        {
            dirUp = true;
            fireDeg = 90;
            if (dirRight)
                firePoint = iPointMake(p.x + 5, p.y - 40);
            else
                firePoint = iPointMake(p.x - 5, p.y - 40);
        }
        else
        {
            dirUp = false;
        }
    }
    if (getKeyDown(keyboard_z))
    {
        topState = PlayerJump;
        botState = PlayerJump;
        topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
        botImgs[botState]->startAnimation(AnimationMgr::cbAniToIdle, this);
        if (jumpCombo == 1)
            return;
        up -= jumpPow;
        jumpCombo++;
    }
    if (getKeyDown(keyboard_x))
        fire(v);
    if (getKeyDown(keyboard_space))
        bomb(v);


    //Animation
    if (!up)
    {
        if (v != iPointZero)
        {
            if (v.x)
            {
                if(!fireing)
                    topState = PlayerWalk;
                botState = PlayerWalk;
            }
            if (v.y>0)
            {
                topState = PlayerCrouch;
            }
            else if (v.y < 0)
            {
                if (!fireing)
                    topState = PlayerAimtoUp;    
            }
        }
        else
        {
            botState = PlayerIdle;
            if (topState == PlayerWalk || topState == PlayerJump || topState == PlayerRunJump)
            {
                topState = PlayerBrake;
                topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
            }
			else if (topState == PlayerCrouch || topState == PlayerAimtoUp)
            {
                topImgs[topState]->startAnimation();
                topState = PlayerIdle;
            }

        }
    }
    else//if(up)
    {
        if (v != iPointZero)
        {
            topState = PlayerRunJump;
            botState = PlayerRunJump;
        }
        else
        {
            topState = PlayerJump;
            botState = PlayerJump;
        }
    }    
    
    p.x += v.x;
    fixedUpdate(dt);
}
void ProcPlayer::fixedUpdate(float dt)
{ 
    int maxY = *(map->maxY + (int)p.x);
    if (p.y > maxY)      //on air
    {
        p.y = maxY;
        up = 0;
        down = 0;
        fall = false;
        jumpCombo = 0;
    }
    else
        fall = true;

    if (up)
    {
        p = (iPointMake(p.x, p.y -= jumpPow));
        up += jumpDecrease * dt;

        if (up > 0.0f)
            up = 0.0f;
    }

    if (fall)
    {
        if (p.y < maxY)
        {
            down += jumpDecrease * dt;
            p = (iPointMake(p.x, p.y += down));
        }
    }

    for (int i = 0; i < colNum; i++)
        colliders[i]->update(this);
}
bool ProcPlayer::draw(float dt, iPoint off)
{
    setRGBA(1, 1, 1, alpha);

    if (curGun->gunIndex == 1)
        topImgs = _imgEriHeavyTop;
    else if (curGun->gunIndex == 0)
        topImgs = _imgEriTop;
    
    
    botImgCurr = botImgs[botState];
    topImgCurr = topImgs[topState];
    if (topState < PlayerBrake)
        botImgCurr->paint(dt, p + off);
    if (!dirRight)
    {
        if (v.x < 0)
            topImgCurr->reverse = REVERSE_WIDTH;
        topImgCurr->paint(dt, p + off);
    }
    else
    {
        if (v.x > 0)
            topImgCurr->reverse = REVERSE_WIDTH;        
        topImgCurr->paint(dt, p + off);
    }    
#if 0
    topImgCurr = topImgs[topState];
    botImgCurr = botImgs[botState];
#endif
    {
#ifdef _DEBUG
        setDotSize(10);
        drawDot(p + off);
        drawRect(colliders[0]->getCollider());
        //drawRect(p.x - 100, p.y - 100, 200, 200);
#endif
    }
    return !isActive;
}

void ProcPlayer::free()
{
}
void ProcPlayer::fire(iPoint v)
{
    topState = PlayerFire;
    fireing = true;
    ProcEnemy* eNear = NULL;
    float dNear = 0xffffff;
    for (int i = 0; i < enemyCount; i++)
    {
        ProcEnemy* e = enemies[i];
        float d = iPointLength(p - e->p);
        if (attkRange > d)
        {
            dNear = d;
            if (!e->isDead)
                eNear = e;
        }
    }

    if (eNear)
    {
        topState = PlayerMeleeAttack;
        eNear->hp -= 100;
        if (eNear->hp <= 0)
            eNear->dead();
    }
    else
    {
        if (v.y < 0)           //aim up
        {
            topState = PlayerFireUp;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
            printf("fireup\n");
        }
        else if (v.y > 0)   //aim down
        {
            topState = PlayerFireDown;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
        }
        else
        {
            topState = PlayerFire;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
            printf("fire!!\n");
        }
        addBullet(this, curGun->gunIndex, fireDeg);
    }
}

void ProcPlayer::bomb(iPoint v)
{
    topState = PlayerBomb;
    int maxY = *(map->maxY + (int)p.x);

    if (v.x > 0)
        bombPoint = iPointMake(p.x + bombRange, maxY);
    else if (v.x < 0)
        bombPoint = iPointMake(p.x - bombRange, maxY);
    addBullet(this, Bomb, -45);
}

void ProcPlayer::dead()
{
    isDead = true;
    if (topState != (PlayerBehave)(PlayerDead + topState % 2))
    {
        ;// setTopState((PlayerBehave)(PlayerDead + topState % 2));
    }
}

void ProcPlayer::getDamage(float damage, Collider* c)
{
    hp -= damage;
    if (hp <= 0)
        if (!isDead)
            dead();
}

void ProcPlayer::addScore(int score)
{
    this->score += score;
}

void ProcPlayer::addBomb(int bomb)
{
    printf("%d\n", bomb);
    this->bombs += bomb;
}

void ProcPlayer::changeGun(int index)
{
    this->curGun->gunIndex = (GunIndex)index;
}

ImageInfo botImageInfo[] =
{
   {
      "assets/Player/Bot_Idle_%02d.png",
      1, 1.0f, { -20 / 2, 0 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Bot_Walk_%02d.png",
      12, 1.0f, { -24 / 2, 0 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Bot_Jump_%02d.png",
      6, 1.0f, { -22 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Bot_RunJump_%02d.png",
      6, 1.0f, { -22 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
};
ImageInfo topImageInfo[] =
{
   {
      "assets/Player/Top_Idle_%02d.png",
      4, 1.0f, { -26 / 2, 10 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Top_Walk_%02d.png",
      12, 1.0f, { -18 / 2, 12 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Top_Jump_%02d.png",
      6, 1.0f, {  -26 / 2, 18 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Top_RunJump_%02d.png",
      6, 1.0f, { -26 / 2, 6 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   //Top
   {
      "assets/Player/AimUp_%02d.png",
      6, 1.0f, { -32 / 2, 16 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/AimtoNorm_%02d.png",
      4, 1.0f, { -26 / 2, 16 },
      0.06f,
      1,
      {255, 0, 0, 255},
      NULL,
   },
   {
      "assets/Player/AimDown_%02d.png",
      3, 1.0f, { -20 / 2, 4 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Top_Fire_%02d.png",
      9, 1.0f, { -26 / 2, 12 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Top_FireUp_%02d.png",
      10, 1.0f, { -34 / 2, 16 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Top_FireDown_%02d.png",
      6, 1.0f, { -30 / 2, -10 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Melee_%02d.png",
      6, 1.0f, { -28 / 2, 6 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Bomb_%02d.png",
      6, 1.0f, { -32 / 2, 8 },
      0.06f,
      1,
      {255,0,0,255},
      AnimationMgr::cbAniToIdle,
   },
   //Whole
   {
      "assets/Player/Brake_%02d.png",
      4, 1.0f, { -20 / 2, 0 },
      0.12f,
      1,
      {255, 255, 255, 255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Crouch_%02d.png",
      11, 1.0f, { -51 / 2, 0 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Crouch_Melee_%02d.png",
      11, 1.0f, { -51 / 2, 0 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Spawn_%02d.png",
      7, 1.0f, { -29 / 2, 0 },
      0.04f,
      1,
      {255,0,0,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Dead_%02d.png",
      19, 1.0f, { -41 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
};

ImageInfo infoEriTopHeavy[] =
{
   {
      "assets/Player/Heavy_Idle_%02d.png",
      4, 1.0f, { -28 / 2, 10 },
      0.18f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_Walk_%02d.png",
      12, 1.0f, { -48 / 2, 12 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_Jump_%02d.png",
      6, 1.0f, { -48 / 2, 12 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_RunJump_%02d.png",
      6, 1.0f, { -58 / 2, 12 },
      0.1f,
      0,
      {255,0,0,255},
      NULL,
   },
   //top
   {
      "assets/Player/Heavy_Crouch_Melee_%02d.png",
      9, 1.0f, { -42 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_AimUp_%02d.png",
      6, 1.0f, { -60 / 2, 16 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_AimtoNorm_%02d.png",
      3, 1.0f, { -60 / 2, 16 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_AimDown_%02d.png",
      3, 1.0f, { -60 / 2, 4 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_Fire_%02d.png",
      7, 1.0f, { -32 / 2, 12 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Heavy_FireUp_%02d.png",
      4, 1.0f, { -76 / 2, 18 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Heavy_FireDown_%02d.png",
      4, 1.0f, { -68 / 2, -36 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Heavy_Melee_%02d.png",
      6, 1.0f, { -76 / 2, 18 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Hevay_Bomb_%02d.png",
      6, 1.0f, { -54 / 2, 10 },
      0.06f,
      1,
      {255,0,0,255},
      AnimationMgr::cbAniToIdle,
   },
   //Whole
   {
      "assets/Player/Heavy_Brake_%02d.png",
      4, 1.0f, { -42 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Heavy_Crouch_%02d.png",
      7, 1.0f, { -42 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
   //Only
   {
      "assets/Player/Spawn_%02d.png",
      7, 1.0f, { -29 / 2, 0 },
      0.06f,
      0,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Dead_%02d.png",
      19, 1.0f, { -41 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
};


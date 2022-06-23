#include "ProcPlayer.h"

#include "iStd.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "BulletMgr.h"
#include "InputMgr.h"
#include "EffectMgr.h"

#include "Proc.h"
#include "ProcField.h"
#include "ProcItem.h"
ProcPlayer* player;
static iImage** _imgEriTop = NULL;
static iImage** _imgEriBot = NULL;
static iImage** _imgEriHeavyTop = NULL;
ImageInfo topImageInfo[];
ImageInfo botImageInfo[];
ImageInfo infoEriTopHeavy[];

ProcPlayer::ProcPlayer(int idx) : ProcObject()
{    
    layer = ObjLayer::LayerPlayer;
    p = iPointZero;
    s = iSizeZero;
    isActive = false;

    this->idx = (CharacterIndex)idx;
    topImgs = NULL;
    topImgCurr = NULL;
    botImgs = NULL;
    botImgCurr = NULL;

    curGun = new Gun();//{ HandGun, 100, 100, 0 };
    //firePoint
    bombPoint = { -1, -1 };
    up = 0;
    down = 0;
    fall = true;

    isAttacking = false;
    isAimup = false;
    isCrouching = false;
    isAimDown = false;

    hp = 100;
    life = 2;
    moveSpeed = 150.f;
    bombSpeed = 10.f;
    attkRange = 40;

    bombs = 20;
    score = 0;

    topState = IdleR;
    botState = IdleR;

    _imgEriTop = createImgReverse(topImageInfo, PlayerBehaveMax, this);
    _imgEriHeavyTop = createImgReverse(infoEriTopHeavy, PlayerBehaveMax, this);
    _imgEriBot = createImgReverse(botImageInfo, 8, this);

    switch (idx)
    {
    case MARCO:   printf("No imgs");                            break;
    case ERI:     topImgs = _imgEriTop; botImgs = _imgEriBot;   break;
    case TAMA:    printf("No imgs");                            break;
    case PIO:     printf("No imgs");                            break;
    }
}

ProcPlayer::~ProcPlayer()
{
    objects->removeObject(this);
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

void ProcPlayer::init()
{
    this->isActive = true;
    topState = IdleR;
    botState = IdleR;
    this->p = iPointMake(100, 200);

    objects->addObject(this);
}

void ProcPlayer::update(float dt)
{
    v = iPointZero;
    firePoint = iPointMake(p.x, p.y - 20);
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
        v /= iPointLength(v);
        v *= (moveSpeed * dt);

        if (v.x < 0)
            isRight = false;
        else if (v.x > 0)
            isRight = true;

        if (!up)
        {
            if (v.x > 0)
            {
                setBotState(WalkR, v);
                if (!isAttacking)
                    setTopState(WalkR, v);
                else
                {
                    if( topState != FireR)
                        setTopState(WalkR, v);
                }

                fireDeg = 0;
            }
            else if (v.x < 0)// if (v.x < 0)
            {                
                setBotState(WalkL, v);
                if (!isAttacking)
                    setTopState(WalkL, v);
                else
                {
                    if (topState != FireL)
                        setTopState(WalkL, v);
                }

                fireDeg = 180;
            }
        }
    }
    else//
    {
        if (topState == WalkR || topState == WalkL
            ||topState==JumpR || topState == JumpL)
            setTopState((PlayerBehave)(BrakeR + topState % 2), v);
    }

    if (getKeyDown(keyboard_z))
        jump(v);
    if (getKeyDown(keyboard_up))
        aimUp();
    if (getKeyDown(keyboard_x))
        fire(v);
    if (getKeyDown(keyboard_space))
        bomb(v);
    else if (getKeyUp(keyboard_up))
        setTopState((PlayerBehave)(AimtoNormR + topState % 2), v);
    if (getKeyDown(keyboard_down))
        crouch();
    else if (getKeyUp(keyboard_down))
        setTopState((PlayerBehave)(IdleR + topState % 2), v);





    p.x += v.x;

    fixedUpdate(dt);
}
void ProcPlayer::fixedUpdate(float dt)
{
#if 1
    //Camera Move
    float x = p.x + bg->off.x;
    if (x < devSize.width / 3)
    {
        bg->move(iPointMake(devSize.width / 3 - x, 0));
    }
    else if (x > devSize.width * 2 / 3)
    {
        bg->move(iPointMake(devSize.width * 2 / 3 - x, 0));
    }
#endif   
    int maxY = *(bg->maxY + (int)p.x);
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
}
bool ProcPlayer::draw(float dt, iPoint off)
{
    setRGBA(1, 1, 1, 1);

    botImgCurr = botImgs[botState];
    if (curGun->gunIndex == 1)
        topImgs = _imgEriHeavyTop;
    else if (curGun->gunIndex == 0)
        topImgs = _imgEriTop;
    topImgCurr = topImgs[topState];

    if (topState < SpawnR)
        botImgCurr->paint(dt, p + off);
    topImgCurr->paint(dt, p + off);
    {
#ifdef _DEBUG
        setRGBA(1, 0, 1, 0.5);
        setDotSize(10);
        drawDot(p + off);
        drawRect(collider->getCollider());
#endif
    }

    //addRecord(RecordStep, p);
    //drawRecord(dt, off);
    setRGBA(1, 1, 1, 1);
#if 0
    if (getKeyDown(keyboard_space))
        addFB(0, p, fireDegree);
    else if (getKeyStat(keyboard_up))
    {
        if (fireDegree < 90)
        {
            fireDegree += (90 / 0.5f * dt);
            if (fireDegree > 90)
                fireDegree = 90;
        }
        else if (fireDegree > 90)
        {
            fireDegree -= (90 / 0.5f * dt);
            if (fireDegree < 90)
                fireDegree = 90;
        }
    }
    drawFB(dt, off);
#endif
    return !isActive;
}

void ProcPlayer::free()
{
}

void ProcPlayer::setTopState(PlayerBehave pb, iPoint v)
{
    if (pb == JumpR || pb == JumpL)
    {
        topImgs[pb]->startAnimation(cbAniToWalk, this);
        botImgs[pb]->startAnimation(cbAniToWalk, this);
    }
    if (pb == RunJumpR || pb == RunJumpL)
    {
        topImgs[pb]->startAnimation(cbAniToWalk, this);
        botImgs[pb]->startAnimation(cbAniToWalk, this);
    }
    if (pb == AimtoUpR || pb == AimtoUpL)
    {
        topImgs[pb]->startAnimation();
    }
    if (pb == AimtoNormR || pb == AimtoUpL)
    {
        topImgs[pb]->startAnimation(cbAniToIdle, this);
    }
    if (pb == AimDownR || pb == AimDownL)
    {
        topImgs[pb]->startAnimation();
    }
    if (pb == FireR || pb == FireL)
    {
        //#issue
        topImgCurr[FireR].frame;//bottom sync

        topImgs[pb]->startAnimation(cbAniFire, this);
    }
    if (pb == FireUpR || pb == FireUpL)
    {
        topImgs[pb]->startAnimation(cbAniFire, this);
    }
    if (pb == FireDownR || pb == FireDownL)
    {
        topImgs[pb]->startAnimation(cbAniFire, this);
    }
    if (pb == MeleeR || pb == MeleeL)
    {
        topImgs[pb]->startAnimation(cbAniFire, this);
    }
    if (pb == BombR || pb == BombL)
    {
        topImgs[pb]->startAnimation(cbAniFire, this);
    }
    if (pb == SpawnR || pb == SpawnL)
    {
        topImgs[pb]->startAnimation();
    }
    if (pb == CrouchR || pb == CrouchL)
    {
        topImgs[pb]->startAnimation();
    }
    if (pb == BrakeR || pb == BrakeL)
    {
        topImgs[pb]->startAnimation(cbAniBrake, this);
    }
    topState = pb;
}

void ProcPlayer::setBotState(PlayerBehave pb, iPoint v)
{
    botState = pb;

}

void ProcPlayer::jump(iPoint v)
{
    if (jumpCombo == 1)
        return;
    if (v.x > 0)
    {
        setTopState(RunJumpR, v);
        setBotState(RunJumpR, v);
    }
    else if (v.x < 0)
    {
        setTopState(RunJumpL, v);
        setBotState(RunJumpL, v);
    }
    else
    {
        setTopState((PlayerBehave)(JumpR + topState % 2), v);
        setBotState((PlayerBehave)(JumpR + botState % 2), v);
    }

    up -= jumpPow;
    jumpCombo++;
}

void ProcPlayer::crouch()
{
    isCrouching = !isCrouching;
    if (!up)
    {
        setTopState((PlayerBehave)(CrouchR + topState % 2), v);
        setBotState((PlayerBehave)(IdleR + botState % 2), v);
    }
    else
    {
        setTopState((PlayerBehave)(AimDownR + topState % 2), v);
        setBotState((PlayerBehave)(JumpR + botState % 2), v);
    }
}

void ProcPlayer::aimUp()
{
    isAimup = !isAimup;
    if (!up)
    {
        setTopState((PlayerBehave)(AimtoUpR + topState % 2), v);
        //setBotState((PlayerBehave)(IdleR + botState % 2), v);
    }
    else
    {
        setTopState((PlayerBehave)(AimtoUpR + topState % 2), v);
        //setBotState((PlayerBehave)(JumpR + botState % 2), v);
    }
}

void ProcPlayer::fire(iPoint v)
{
    isAttacking = true;
    ProcEnemy* eNear = NULL;
    float dNear = 0xffffff;
    for (int i = 0; i < enemyCount; i++)
    {
        ProcEnemy* e = enemies[i];
        float d = iPointLength(p - e->p);
        if (attkRange > d)
        {
            dNear = d;
            if (e->getState() != (EnemyBehave)(DeadEnemyL + e->state % 2))
                eNear = e;
        }
    }    

    if (eNear)
    {
        setTopState((PlayerBehave)(MeleeR + topState % 2), v);
        eNear->hp -= 100;
        if (eNear->hp <= 0)
            eNear->setState((EnemyBehave)(DeadEnemyL + eNear->state % 2));
    }
    else
    {
        if (v.y < 0)           //aim up
        {
            setTopState((PlayerBehave)(FireUpR + topState % 2), v);
            fireDeg = 270;
        }
        else if (v.y > 0)   //aim down
        {
            setTopState((PlayerBehave)(FireDownR + topState % 2), v);
            fireDeg = 90;
        }
        else
            setTopState((PlayerBehave)(FireR + topState % 2), v);

        addBullet(this, curGun->gunIndex, fireDeg);
    }

    if (!up)
    {
        if (v.x > 0)
            setBotState(WalkR, v);
        else if (v.x < 0)
            setBotState(WalkL, v);
        else
            setBotState((PlayerBehave)(IdleR + botState % 2), v);
    }
}

void ProcPlayer::bomb(iPoint v)
{
    setTopState((PlayerBehave)(BombR + topState % 2), v);

    addBullet(this, Bomb, 45);
}

void ProcPlayer::dead()
{
    topState = (PlayerBehave)(DeadEnemyL + topState % 2);
    topImgs[topState]->startAnimation(AnimationMgr::cbAniDead, this);
}

void ProcPlayer::getDamage(float damage)
{
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

void ProcPlayer::cbAniToIdle(void* parm)
{
    ProcPlayer* pp = (ProcPlayer*)parm;
    pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
    pp->setBotState((PlayerBehave)(IdleR + pp->botState % 2), iPointZero);
}

void ProcPlayer::cbAniToWalk(void* parm)
{
    ProcPlayer* pp = (ProcPlayer*)parm;
    if (pp->v.x)
    {
        pp->setTopState((PlayerBehave)(WalkR + pp->topState % 2), iPointZero);
        pp->setBotState((PlayerBehave)(WalkR + pp->botState % 2), iPointZero);
    }
}

void ProcPlayer::cbAniBrake(void* parm)
{
    ProcPlayer* pp = (ProcPlayer*)parm;
    pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
    pp->setBotState((PlayerBehave)(IdleR + pp->botState % 2), iPointZero);
}

void ProcPlayer::cbAniFire(void* parm)
{
    ProcPlayer* pp = (ProcPlayer*)parm;
    pp->isAttacking = false;
    pp->setTopState((PlayerBehave)(IdleR + pp->topState % 2), iPointZero);
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
   {
      "assets/Player/Brake_%02d.png",
      4, 1.0f, { -20 / 2, 0 },
      0.12f,
      1,
      {255, 255, 255, 255},
      ProcPlayer::cbAniBrake,
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
      ProcPlayer::cbAniToIdle,
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
      ProcPlayer::cbAniFire,
   },
   {
      "assets/Player/Top_FireUp_%02d.png",
      10, 1.0f, { -34 / 2, 16 },
      0.06f,
      1,
      {255,255,255,255},
      ProcPlayer::cbAniFire,
   },
   {
      "assets/Player/Top_FireDown_%02d.png",
      6, 1.0f, { -30 / 2, -10 },
      0.06f,
      1,
      {255,255,255,255},
      ProcPlayer::cbAniFire,
   },
   {
      "assets/Player/Melee_%02d.png",
      6, 1.0f, { -28 / 2, 6 },
      0.06f,
      1,
      {255,255,255,255},
      ProcPlayer::cbAniFire,
   },
   {
      "assets/Player/Bomb_%02d.png",
      6, 1.0f, { -32 / 2, 8 },
      0.06f,
      1,
      {255,0,0,255},
      ProcPlayer::cbAniFire,
   },
   {
      "assets/Player/Spawn_%02d.png",
      7, 1.0f, { -29 / 2, 0 },
      0.04f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Dead_%02d.png",
      19, 1.0f, { -41 / 2, 0 },
      0.04f,
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
   {
      "assets/Player/Heavy_Brake_%02d.png",
      4, 1.0f, { -42 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      ProcPlayer::cbAniBrake,
   },
   {
      "assets/Player/Heavy_Crouch_%02d.png",
      7, 1.0f, { -42 / 2, 0 },
      0.1f,
      1,
      {255,0,0,255},
      NULL,
   },
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
      NULL,
   },
   {
      "assets/Player/Heavy_FireUp_%02d.png",
      4, 1.0f, { -76 / 2, 18 },
      0.06f,
      1,
      {255,255,255,255},
      NULL,
   },
   {
      "assets/Player/Heavy_FireDown_%02d.png",
      4, 1.0f, { -68 / 2, -36 },
      0.06f,
      1,
      {255,255,255,255},
      NULL,
   },
   {
      "assets/Player/Heavy_Melee_%02d.png",
      6, 1.0f, { -76 / 2, 18 },
      0.06f,
      1,
      {255,255,255,255},
      NULL,
   },
   {
      "assets/Player/Hevay_Bomb_%02d.png",
      6, 1.0f, { -54 / 2, 10 },
      0.06f,
      1,
      {255,0,0,255},
      NULL,
   },
   ///////////////////////////////////////////
   {
      "assets/Player/Spawn_%02d.png",
      7, 1.0f, { -29 / 2, 0 },
      0.04f,
      1,
      {255,0,0,255},
      NULL,
   },
   {
      "assets/Player/Dead_%02d.png",
      19, 1.0f, { -41 / 2, 0 },
      0.04f,
      1,
      {255,0,0,255},
      NULL,
   },
};


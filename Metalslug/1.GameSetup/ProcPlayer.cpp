#include "ProcPlayer.h"

#include "UIMgr.h"
#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "BulletMgr.h"
#include "InputMgr.h"

#include "Proc.h"
#include "ProcField.h"
#include "ProcItem.h"
#include "ProcStructure.h"

ProcPlayer* player;
ImageInfo topImageInfo[];
ImageInfo botImageInfo[];
ImageInfo infoEriTopHeavy[];
static iImage** _imgEriTop = NULL;
static iImage** _imgEriBot;
static iImage** _imgEriHeavyTop;
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

    topImgs = NULL;
    topImgCurr = NULL;
    botImgs = NULL;
    botImgCurr = NULL;
    degree = 0.f;

    curGun = new Gun();//{ HandGun, 100, 100, 0 };
    fireDeg = 0;
    tp = p;
    fp = iPointMake(p.x + 15, p.y - 20);
    bombPoint = p;

    up = 0;
    down = 0;
    fall = true;

    fireing = false;
    dirUp = false;

    fireDeg = 0.f;    
    isDead = false;
    jumpCombo = 0;

    maxX = 0.f;
    maxY = 0.f;

    topState = PlayerIdle;
    botState = PlayerIdle;

    hp = 0.f;
    life = 3;
    moveSpeed = 120.f;
    attkRange = 30;

    bombs = 10;
    score = 0;
#if 1
    alpha = 1.0f;
    inviDt = 0.0f, _inviDt = 3.0f;

    canControl = true;
#endif

    _imgEriTop = createSingleImage(topImageInfo, PlayerBehaveMax, this);
    for (int i = 0; i < PlayerBehaveMax; i++)
    {
            printf("_imgEritop[i]=%p\n", _imgEriTop[i]);
    }
    _imgEriHeavyTop = createSingleImage(infoEriTopHeavy, PlayerBehaveMax, this);
    _imgEriBot = createSingleImage(botImageInfo, 8, this);
    switch (index)
    {
    case MARCO:   printf("No imgs");                            break;
    case ERI:     topImgs = _imgEriTop; botImgs = _imgEriBot;         break;
    case TAMA:    printf("No imgs");                            break;
    case PIO:     printf("No imgs");                            break;
    }

    topImgCurr = _imgEriTop[topState];
    botImgCurr = _imgEriBot[botState];
}

ProcPlayer::~ProcPlayer()
{
    if (_imgEriTop)
    {
        delete _imgEriTop;
        _imgEriTop = NULL;
        delete _imgEriBot;
        delete _imgEriHeavyTop;
    }
    delete curGun;
    for (int i = 0; i < rectNum; i++)
        delete rect[i];
    delete rect;

}

void ProcPlayer::init(iPoint p)
{
    isActive = true;
    isDead = false;
    dirRight = true;
    dirUp = false;
    fireDeg = 0;
    tp = p;
    fp = iPointMake(p.x + 15, p.y - 22);
    topState = PlayerSpawn;
    botState = PlayerIdle;
    this->p = p;

    rectNum = 1;   
    rect = new iRect * [rectNum];
    for (int i = 0; i < rectNum; i++)
        rect[i] = new iRect();

    for (int i = 0; i < rectNum; i++)
    {
        iRect* r = rect[i];
        r->size = iSizeMake(40, 40);
        r->origin = p;
    }    
    curGun->gunIndex = Handgun;
    topImgs = _imgEriTop;
    botImgs = _imgEriBot;
    alpha = 1.0f;
    bombs = 10;
    hp = 100;
    life--;

    if (topState == PlayerSpawn)
        topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
}

static float count;
static float _count = 5.f;
static float delta;
static float _delta = 0.1f;
static float d;
void ProcPlayer::update(float dt)
{   
    if (isDead || !canControl)
        return;

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
            fp = iPointMake(p.x - 15, p.y - 22);
        }
        else if (v.x > 0)
        {
            dirRight = true;         
            fireDeg = 0;
            fp = iPointMake(p.x + 15, p.y - 22);
        }

        if (v.y < 0)
        {
            dirUp = false;
            fireDeg = 270;
            if (!up)
            {
                if (dirRight)
                    fp = iPointMake(p.x + 1, p.y - 80);  //fireUp right
                else
                    fp = iPointMake(p.x - 1, p.y - 80);  //fireUp left
            }
        }
        else if (v.y > 0)
        {
            dirUp = true;
            if (!up)
            {
                fireDeg = dirRight ? 0 : 180;
                fp = iPointMake(p.x + 15, p.y - 18);     //crouch Fire
            }
            else//if(up)
            {
                if (curGun->gunIndex == Handgun)
                {
                    if (dirRight)
                        fp = iPointMake(p.x + 1, p.y - 5);   //fireDown right
                    else
                        fp = iPointMake(p.x - 1, p.y - 5);   //fireDown left
                    fireDeg = 90;
                }
                else
                {
                    if (dirRight)
                    {            
#if 0
                        if (delta)  //firing
                        {
                            d += dt;
                            if (d > 1)
                                d -= 1;
                        }
#endif
                        fp = iPointMake(p.x + 1, p.y - 5);   //fireDown left
                        fireDeg = 90;
                    }
                    else
                    {
                        fp = iPointMake(p.x - 1, p.y - 5);   //fireDown left
                        fireDeg = 90;
                    }
                }
            }
        }
        else
            dirUp = false;
    }
    else
    {
        //fireDeg init
        if (dirRight)
        {
            fireDeg = 0;
            fp = iPointMake(p.x + 15, p.y - 25);
        }
        else// if(!dirRight)
        {
            fireDeg = 180;
            fp = iPointMake(p.x - 15, p.y - 25);
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
    

    //check firing
    if (count)
    {
        delta += dt;
        if (delta > _delta)
        {
            float deg = fireDeg + (rand() % 2 - 2) * 2;
            addBullet(this, HeavyMachinegun, deg);
            curGun->remain--;
            delta = 0.f;
            count++;            
        }
        if (curGun->remain < 1)
            changeGun(Handgun);
        if (count >= _count)
            count = 0;
    }
    //Animation
    if (!up)
    {
        if (v != iPointZero)
        {
            if (v.x)
            {
                if (!fireing)
                    topState = PlayerWalk;
                botState = PlayerWalk;
            }
            else
                botState = PlayerIdle;
            if (v.y>0)
            {
                if (!fireing)
                    topState = PlayerCrouch;
                else
                    topState = PlayerCrouchFire;
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
			else if (topState == PlayerCrouch || topState==PlayerCrouchFire || topState == PlayerAimtoUp)
            {
                topImgs[topState]->startAnimation();
                topState = PlayerIdle;
            }

        }
    }
    else//if(up)
    {
        if (v.y > 0)
        {
            if (!fireing)
                topState = PlayerAimtoDown;
            else
                topState = PlayerFireDown;
        }
        else if (v.y < 0)
        {
            if (!fireing)
                topState = PlayerAimtoUp;
            else
                topState = PlayerFireUp;
        }
        else
        {
            if (v.x != 0)
            {
                if (fireing)
                    topState = PlayerFire;
                else
                    topState = PlayerRunJump;
                botState = PlayerRunJump;
            }
            else
            {
                if (fireing)
                    topState = PlayerFire;
                else
                    topState = PlayerJump;
                botState = PlayerJump;
            }
        }        
    }    
    p.x += v.x;
    //fixedUpdate(dt);
}

void ProcPlayer::fixedUpdate(float dt)
{ 
    int maxH = *(map->maxY + (int)p.x);
    if (p.y > maxH)      //on air
    {
        p.y = maxH;
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
        if (p.y < maxH)
        {
            down += jumpDecrease * dt;
            p = (iPointMake(p.x, p.y += down));
        }
    }

    //ColliderUpdate
    for (int i = 0; i < rectNum; i++)
    {
        rect[i]->origin = iPointMake(
            p.x + map->off.x - rect[i]->size.width / 2,
			p.y + map->off.y - rect[i]->size.height);
    }    
}

bool ProcPlayer::draw(float dt, iPoint off)
{
    setRGBA(1, 1, 1, alpha);

    if (curGun->gunIndex == 1)
        topImgs = _imgEriHeavyTop;
    else if (curGun->gunIndex == 0)
        topImgs = _imgEriTop;
    
    
    if (topState == 17)
        topState = PlayerIdle;            
    botImgCurr = botImgs[botState];
    topImgCurr = topImgs[topState];
    topImgCurr->reverse = dirRight ? REVERSE_NONE : REVERSE_WIDTH;
    botImgCurr->reverse = dirRight ? REVERSE_NONE : REVERSE_WIDTH;
    if (topState < PlayerBrake)
        botImgCurr->paint(dt, p + off);
    topImgCurr->paint(dt, p + off);

    if (inviDt)
    {
        inviDt += dt;
        alpha = fabsf(_cos((inviDt / _respawnDt * 540)));        //blink 3 time
        if (inviDt > _inviDt)
            inviDt = 0.0f;
    }
#ifdef DEBUG
    for (int i = 0; i < rectNum; i++)
        drawRect(getRect());           
#endif // _DEBUG


    setRGBA(1, 1, 1, 1);
    fixedUpdate(dt);
    return !isActive;
}

void ProcPlayer::free()
{
}

void ProcPlayer::fire(iPoint v)
{
    if (curGun->gunIndex == HeavyMachinegun)
    {
        if (curGun->remain > 0)
            curGun->remain--;

        if (curGun->remain <= 0)
            curGun->changeGun(Handgun);
    }
    topState = PlayerFire;
    fireing = true;

#if 1
    ProcObject* dst = NULL;
    float dMin = 0xfffff;
    for (int i = 0; i < enemyNum; i++)
    {
        ProcEnemy* e = enemies[i];
        float d = iPointLength(e->p - p); 
        if (dMin > d)
        {
            if (d < attkRange)
            {
                dMin = d;
				if (e->getRect().size.width != 0 && e->getRect().size.height != 0)
                    dst = e;                    
            }
        }
    }

    if (dst)
    {
        if (dst->getRect().size.width == 0 || dst->getRect().size.height == 0)
            return;
        topState = PlayerMeleeAttack;
        audioPlay(snd_eff_melee);
        topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
        dst->getDamage(100);       
    }
    else
    {
        if (curGun->gunIndex == Handgun)
            audioPlay(snd_eff_fire);
        if (curGun->gunIndex == HeavyMachinegun)
            audioPlay(snd_eff_HMG_fire);
        if (v.y < 0)           //aim up
        {
            topState = PlayerFireUp;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);            
        }
        else if (v.y > 0)   //aim down
        {
            if(up)
                topState = PlayerFireDown;
            else
                topState=PlayerCrouchFire;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
        }
        else
        {
            topState = PlayerFire;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);            
        }
        if(curGun->gunIndex==Handgun)
            addBullet(this, curGun->gunIndex, fireDeg);   
        else if (curGun->gunIndex == HeavyMachinegun)
        {
            addBullet(this, curGun->gunIndex, fireDeg);
            count = 1;
            delta = 0.05f;
        }
    }
#endif
}

void ProcPlayer::bomb(iPoint v)
{
    topState = PlayerBomb;
    topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
    if (bombs < 1)
        return;  
    bombs--;
    int maxY = *(map->maxY + (int)p.x);

    if (v.x > 0)
        bombPoint = iPointMake(p.x + bombRange, maxY);
    else if (v.x < 0)
        bombPoint = iPointMake(p.x - bombRange, maxY);
    addBullet(this, BulletBomb, 0);
}


void ProcPlayer::getDamage(float damage)
{
    if (inviDt)
        return;

    hp -= damage;
    if (hp <= 0)
    {
        if (!isDead)
        {
            isDead = true;
            topState = PlayerDead;
            topImgs[topState]->startAnimation(AnimationMgr::cbAniDead, this);
        }
	}
}

void ProcPlayer::addScore(int score)
{
    this->score += score;
}

void ProcPlayer::addBomb(int bomb)
{    
    this->bombs += bomb;
}

void ProcPlayer::changeGun(int index)
{
    this->curGun->gunIndex = (GunIndex)index;
    if (index == HeavyMachinegun)
        curGun->remain += 200;
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
      { 255,0,0,255 },
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
      "assets/Player/Crouch_Fire_%02d.png",
      9, 1.0f, { -51 / 2, 0 },
      0.06f,
      1,
      {255,0,0,255},
      AnimationMgr::cbAniToCrouch,
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
      AnimationMgr::cbAniDead,
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
      3, 1.0f, { -60 / 2, 0 },
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
      4, 1.0f, { -68 / 2, -28 },
      0.06f,
      1,
      {255,255,255,255},
      AnimationMgr::cbAniToIdle,
   },
   {
      "assets/Player/Heavy_Melee_%02d.png",
      6, 1.0f, { -76 / 2, 14 },
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
   {
       "assets/Player/Heavy_Crouch_Fire_%02d.png",
       4, 1.0f, { -42 / 2, 0 },
       0.06f,
       1,
       { 255,255,255,255 },
       AnimationMgr::cbAniToCrouch,
   },
   //Only
   {
      "assets/Player/Spawn_%02d.png",
      7, 1.0f, { -29 / 2, 0 },
      0.06f,
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
      AnimationMgr::cbAniDead,
   },
};

//
//iPoint moveDuration(iPoint s, iPoint e, float duration, float _duration)
//{
//    float rate = duration / _duration;
//    return moveDuration(s, e, rate);
//}
//
//iPoint moveDuration(iPoint s, iPoint e, float rate)
//{
//    iPoint p;
//    p.x = s.x * (1 - rate) + e.x * rate;
//    p.y = s.y * (1 - rate) + e.y * rate;
//
//    return p;
//}
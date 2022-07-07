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


    topImgs = NULL;
    topImgCurr = NULL;
    botImgs = NULL;
    botImgCurr = NULL;
    degree = 0.f;

    curGun = new Gun();//{ HandGun, 100, 100, 0 };
    fireDeg = 0;
    fp = iPointMake(p.x + 15, p.y - 20);
    bombPoint = p;
    up = 0;
    down = 0;
    fall = true;

    isDead = false;
    fireing = false;
    dirUp = false;

    life = 3;
    moveSpeed = 120.f;
    bombSpeed = 10.f;
    attkRange = 40;
    bombRange = 100.0f;
    bombs = 10;

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
    case ERI:     topImgs = _imgEriTop; botImgs = _imgEriBot;         break;
    case TAMA:    printf("No imgs");                            break;
    case PIO:     printf("No imgs");                            break;
    }

    topImgCurr = _imgEriTop[topState];
    botImgCurr = _imgEriBot[botState];
}

ProcPlayer::~ProcPlayer()
{
    for (int i = 0; i < PlayerBehaveMax; i++)
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
    dirRight = true;
    dirUp = false;
    fireDeg = 0;
    fp = iPointMake(p.x + 15, p.y - 22);
    topState = PlayerSpawn;
    botState = PlayerIdle;
    this->p = p;
    curGun->gunIndex = Handgun;
    //curGun->gunIndex = HeavyMachinegun;
    topImgs = _imgEriTop;
    botImgs = _imgEriBot;
    alpha = 1.0f;
    bombs = 10;
    hp = 100;
    life--;

    if (topState == PlayerSpawn)
        topImgs[topState]->startAnimation(AnimationMgr::cbAniToIdle, this);
#if 1
    colNum = 1;
    for (int i = 0; i < colNum; i++)
    {
        colliders[i]->init(this, iSizeMake(40, 40));
        objects->addObject(colliders[i]);
    }
#endif
}

void ProcPlayer::update(float dt)
{   
    v = iPointZero;
    if (getKeyStat(keyboard_left))
        v.x = -1;
    else if (getKeyStat(keyboard_right))
    {
        v.x = 1;
        printf("player->p.x=%f\n", p.x);
    }
    if (getKeyStat(keyboard_up))
        v.y = -1;
    else if (getKeyStat(keyboard_down))
        v.y = 1;

    if (v != iPointZero)
    {
        v *= moveSpeed * dt;
        //v.setLength(moveSpeed * dt);
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
                if (dirRight)
                    fp = iPointMake(p.x + 1, p.y - 5);   //fireDown right
                else
                    fp = iPointMake(p.x - 1, p.y - 5);   //fireDown left
                fireDeg = 90;
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
                {
                    topState = PlayerCrouchFire;                    
                }
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
    
    fixedUpdate(dt);
}

Collider* cNear = NULL;
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

#if 0
    {
        for (int i = 0; i < objects->count; i++)
        {
            Collider* c = (Collider*)objects->objectAtIndex(i);
            if (c->parent != NULL && c->parent->layer != LayerPlayer)
            {
                printf("%f\n", (c->p.x - c->s.width / 2));
                printf("%f\n", (c->p.x + c->s.width / 2));
                if (p.x > c->p.x - c->s.width / 2 &&
                    p.x < c->p.x + c->s.width / 2 &&
                    p.y > c->p.y - c->s.height)
                {
                    fall = false;
                    p.y = c->p.y - c->s.height;
                }
            }
        }
    }
#endif

    for (int i = 0; i < colNum; i++)
        colliders[i]->update(p, degree, dt);   
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

#if 0
    topImgCurr = topImgs[topState];
    botImgCurr = botImgs[botState];
#endif
    {
#ifdef _DEBUG
        setDotSize(10);
        drawDot(p + off);
        iRect c = colliders[0]->getCollider();
        c.origin.x += off.x;
        c.origin.y += off.y;
        drawRect(c);
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
            printf("fire!!\n");
        }
        addBullet(this, curGun->gunIndex, fireDeg);
        if (curGun->gunIndex == HeavyMachinegun)
        {
            for(int i=0;i<2;i++)
                addBullet(this, curGun->gunIndex, fireDeg);
        }
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
    addBullet(this, BulletBomb, 0);
}


void ProcPlayer::getDamage(float damage, Collider* c)
{
    hp -= damage;
    if (hp <= 0)
    {
        if (!isDead)
        {
            isDead = true;
            topState = PlayerDead;
        }
	}
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
      11, 1.0f, { -51 / 2, 0 },
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

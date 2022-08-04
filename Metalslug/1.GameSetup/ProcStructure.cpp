#include "ProcStructure.h"

#include "ImgMgr.h"

#include "ProcObject.h"
static iImage** _imgStructure = NULL;
ImageInfo imgStructureInfo[];
ProcStructure::ProcStructure() : ProcObject()
{
    score = 100;
    layer = LayerFieldObj;
    
    imgs = NULL;
    imgCurr = NULL;

    isAppear = false;

    if (_imgStructure == NULL)
        _imgStructure = createSingleImage(imgStructureInfo, StructureIndexMax, this);
    imgs = new iImage * [StructureIndexMax];
    memset(imgs, 0x00, sizeof(iImage*) * StructureIndexMax);
    for (int i = 0; i < StructureIndexMax; i++)
        imgs[i] = _imgStructure[i]->clone();

    rectNum = 1;
    rect = new iRect * [rectNum];
    for (int i = 0; i < rectNum; i++)
        rect[i] = new iRect();
}

ProcStructure::~ProcStructure()
{
}

void ProcStructure::init(int index, iPoint p)
{
    this->isActive = true;
    this->index = index;
    this->p = p;

    iSize size[3];
    iPoint pos[3];
    if (index == AppleStair)
    {
        size[0] = { 80, 10 };
        size[1] = { 80, 10 };
        pos[0] = { p.x - 40, p.y - 30 };
        pos[1] = { p.x + 20, p.y - 70 };
    }
    else if (index == Wall)
    {      
        _hp = 1000;
        hp = _hp;
        imgs[Wall]->stopAnimation();
        imgs[Wall]->frame = 0;

        for (int i = 0; i < rectNum; i++)
        {
            iRect* r = rect[i];
            r->size = iSizeMake(40, 120);
            r->origin = p;
        }
    }
    else if (index == FootStep)
    {
        int r = rand() % 4;
        imgs[FootStep]->stopAnimation();
        imgs[FootStep]->frame = r;
    }
}

void ProcStructure::update(float dt)
{
    if (index == Wall)
    {
        if (containPoint(p,
            iRectMake(-map->off.x - 40, -map->off.y,
                devSize.width, devSize.height)))
        {
            isAppear = true;            
            map->isClipped = true;    

            float x = player->p.x + map->_off.x;
            map->move(iPointMake((devSize.width * 3 - x), map->_off.y));
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

bool ProcStructure::draw(float dt, iPoint off)
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
    imgCurr = imgs[index];
    imgCurr->paint(dt, p + off);

#ifdef _DEBUG
    for (int i = 0; i < rectNum; i++)
        drawRect(getRect(i));
#endif // _DEBUG

    setRGBA(1, 1, 1, 1);
    return !isActive;
}

void ProcStructure::free()
{
}

void ProcStructure::getDamage(float damage)
{
    if (hp <= 0)
        return;

    hp -= damage;
    player->addScore(score);
    dmgDt = 0.000001f;
    if (hp <= _hp * 0.5)
    {
        if (index == Wall)
        {
            imgs[Wall]->frame = 1;
            if (hp <= 0)
            {
                map->imgs[3]->alpha = 1.0f;
                map->isClipped = false;
            }
        }
        if (hp <= 0)
        {
            isActive = false;
            addProcEffect(this, EffectExplosionL, p);
        }
    }

}

#define maxStruct 2
ProcStructure*** _structures;
ProcStructure** structures;
int structNum;
void loadStructure()
{
    _structures = new ProcStructure ** [StructureIndexMax];
    for (int i = 0; i < StructureIndexMax; i++)
    {
        _structures[i] = new ProcStructure*[maxStruct];
        for (int j = 0; j < maxStruct; j++)
            _structures[i][j] = new ProcStructure();
    }
    structures = new ProcStructure * [StructureIndexMax * maxStruct];
    structNum = 0;

    addStructure(AppleStair, iPointMake(630, 180));
    addStructure(Wall, iPointMake(2580, 200));
}

void freeStructure()
{
    for (int i = 0; i < StructureIndexMax; i++)
    {
        for(int j=0; j< maxStruct; j++)
            delete _structures[i][j];
        delete _structures[i];
    }
    delete _structures;
    delete structures;
}

void drawStructure(float dt, iPoint off)
{
    for (int i = 0; i < structNum; i++)
    {
        ProcStructure* s = structures[i];
        s->update(dt);
        if (s->draw(dt, off))
        {
            structNum--;
            structures[i] = structures[structNum];
            i--;
        }
    }
}

void addStructure(int index, iPoint p)
{
    for (int i = 0; i < maxStruct; i++)
    {
        ProcStructure* s = _structures[index][i];
        if (s->isActive == false)
        {
            s->init(index, p);
            structures[structNum] = s;
            structNum++;
            return;
        }
    }
}


ImageInfo imgStructureInfo[] =
{
    {
        "assets/Structure/Apple.png",
        1, 1.0f, { -164 / 2, 0},
        0.18f,
        1,
        {248, 0, 248, 255},
        NULL,
    },
    {
        "assets/Structure/Wall_%02d.png",
        3, 1.0f, { -104 / 2, 0},
        0.18f,
        1,
        {248, 0, 248, 255},
        NULL,
    },
    {
        "assets/Structure/FootStep_%02d.png",
        4, 1.0f, { -8 / 2, 0},
        0.18f,
        1,
        {248, 0, 248, 255},
        NULL,
    },
};
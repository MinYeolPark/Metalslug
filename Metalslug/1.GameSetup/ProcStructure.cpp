#include "ProcStructure.h"

#include "ImgMgr.h"

#include "ProcObject.h"
static iImage** _imgStructure = NULL;
ImageInfo imgStructureInfo[];
ProcStructure::ProcStructure() : ProcObject()
{
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
        //colNum = 1;         
        size[0] = { 50,130 };
        pos[0] = p;
        _hp = 1000;
        hp = _hp;
        imgs[Wall]->stopAnimation();
        imgs[Wall]->frame = 0;
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
    if (index == FootStep)
        printf("draw footstep~\n");
}

bool ProcStructure::draw(float dt, iPoint off)
{
    setRGBA(1, 1, 1, 1);

    imgCurr = imgs[index];
    imgCurr->paint(dt, p + off);
    if (index == FootStep)
        printf("draw footstep~\n");
    setRGBA(1, 1, 1, 1);
    return !isActive;
}

void ProcStructure::free()
{
}

void ProcStructure::getDamage(float damage, Collider* c)
{
    if (hp <= 0)
        return;

    hp -= damage;
    if (hp <= _hp * 0.5)
    {
        if (index == Wall)
            imgs[Wall]->frame = 1;
        if (hp <= 0)
        {
            isActive = false;
            addProcEffect(this, EffectExplosionL, p);
        }
    }

}

#define maxStruct 2
ProcStructure** _structures;
ProcStructure** structures;
int structNum;
void loadStructure()
{
    _structures = new ProcStructure * [StructureIndexMax];
    for (int i = 0; i < StructureIndexMax; i++)
        _structures[i] = new ProcStructure[maxStruct];
    structures = new ProcStructure * [StructureIndexMax * maxStruct];
    structNum = 0;

    addStructure(AppleStair, iPointMake(630, 180));
    addStructure(Wall, iPointMake(2700, 200));
}

void freeStructure()
{
    for (int i = 0; i < StructureIndexMax; i++)
    {
        delete _structures[i];
        delete structures[i];
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
        ProcStructure* s = &_structures[index][i];
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
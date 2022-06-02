#include "MoskTower.h"

#include "ImgMgr.h"
#include "EnemyMgr.h"

#include "ProcBullets.h"

ImageInfo imageInfo[];
MoskTower::MoskTower()
{
	state = mtIdle;
	p = iPointZero;

	iPoint tp[3] = { 
		{p.x - 50 * imageInfo[11].s, p.y - 80 * imageInfo[11].s}, 
		{p.x + 50 * imageInfo[11].s, p.y - 80 * imageInfo[11].s},
		{p.x + 140 * imageInfo[11].s, p.y - 80 * imageInfo[11].s} };
	memcpy(towerP, tp, sizeof(iPoint) * 3);
	memset(doorOpen, 0x00, sizeof(bool));
	
	_rate = 2.f;
	for (int i = 0; i < 3; i++)
	{
		float r = rand() % 2;
		rate[i] = r;
	}
}

MoskTower::~MoskTower()
{
	for (int i = 0; i < 3; i++)
	{
		delete imgDoor[i];
		//delete imgCurtain[i];
	}
	delete imgDoor;
	//delete imgCurtain;

}
void MoskTower::initObj()
{	
	Texture* tex;
	ImageInfo* ii;	
	iImage** _imgBase = new iImage * [MidBossBehaveMax];
	for (int i = 0; i < MidBossBehaveMax; i++)
	{
		iImage* _img = new iImage();
		ii = &imageInfo[i];		
		for (int j = 0; j < ii->num; j++)
		{
			tex = createImage(ii->path);
			_img->addObject(tex);
			freeImage(tex);
		}
		_imgBase[i] = _img;
		_imgBase[i]->position = ii->p;
		_imgBase[i]->scale = ii->s;
		imgBase[i] = _imgBase[i]->clone();
	}

	iImage** _imgAdd = new iImage * [MidBossBehaveMax];
	for (int i = 0; i < MidBossBehaveMax; i++)
	{
		iImage* _img = new iImage();
		ii = &imageInfo[i + 2];
		for (int j = 0; j < ii->num; j++)
		{			
			tex = createImage(iColor4bMake(255,0,0,255), ii->path, j);
			_img->addObject(tex);
			freeImage(tex);
		}
		_imgAdd[i] = _img;
		_imgAdd[i]->position = ii->p;
		_imgAdd[i]->scale = ii->s;
		_imgAdd[i]->startAnimation(ii->cbAni, this);		
		imgAdd[i] = _imgAdd[i]->clone();
	}

	iImage* _imgDoor = new iImage();
	ii = &imageInfo[4];
	for (int i = 0; i < ii->num; i++)
	{
		tex = createImage(iColor4bMake(63, 72, 204, 255), ii->path, i + 1);
		_imgDoor->addObject(tex);
		freeImage(tex);

		_imgDoor->position = ii->p;
		_imgDoor->scale = ii->s;
		_imgDoor->_repeatNum = ii->repeatNum;

		for (int j = 0; j < 3; j++)
		{
			imgDoor[j] = _imgDoor->clone();
			imgDoor[j]->startAnimation(cbAniDoorOpen, this);
		}
	}

	iImage* _imgMis = new iImage();
	ii = &imageInfo[5];
	for (int i = 0; i < ii->num; i++)
	{
		tex = createImage(iColor4bMake(255, 0, 0, 255), ii->path, i);
		_imgMis->addObject(tex);
		freeImage(tex);

		for (int j = 0; j < 3; j++)
		{
			imgMis[j] = _imgMis->clone();
			imgMis[j]->position = ii->p;
			imgMis[j]->scale = ii->s;
			imgMis[j]->_aniDt = ii->aniDt;
			imgMis[j]->startAnimation();
		}
	}
	iImage* _imgTrail = new iImage();
	ii = &imageInfo[6];
	for (int i = 0; i < ii->num; i++)
	{
		tex = createImage(iColor4bMake(63, 72, 204, 255), ii->path, i);
		_imgTrail->addObject(tex);
		freeImage(tex);

		for (int j = 0; j < 3; j++)
		{
			imgTrail[j] = _imgTrail->clone();
			imgTrail[j]->position = ii->p;
			imgTrail[j]->scale = ii->s;
			imgTrail[j]->_aniDt = ii->aniDt;
			imgTrail[j]->startAnimation();
		}
	}
	iImage** _imgCurtain = new iImage * [MidBossBehaveMax];
	for (int i = 0; i < MidBossBehaveMax; i++)
	{
		ii = &imageInfo[i + 7];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{			
			tex = createImage(iColor4bMake(32,8,0,255),ii->path, j + 1);
			img->addObject(tex);
			freeImage(tex);

			img->position = ii->p;
			img->scale = ii->s;
			img->_aniDt = ii->aniDt;
			img->_repeatNum = ii->repeatNum;
			img->startAnimation();
			_imgCurtain[i] = img;
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MidBossBehaveMax; j++)
		{
			imgCurtain[i][j] = _imgCurtain[j]->clone();			
		}
		ctState[i] = mtIdle;
	}
	iImage** _imgSoldier = new iImage * [MidBossBehaveMax];
	for (int i = 0; i < MidBossBehaveMax; i++)
	{
		ii = &imageInfo[i + 9];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{
			tex = createImage(iColor4bMake(255,0,0,255), ii->path, j + 1);
			img->addObject(tex);
			freeImage(tex);
		}
		_imgSoldier[i] = img;
	}	
	iImage* img = new iImage();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MidBossBehaveMax; j++)
		{
			img = _imgSoldier[j]->clone();
			img->position = ii->p;
			img->scale = ii->s;
			img->_aniDt = ii->aniDt;
			img->_repeatNum = ii->repeatNum;

			imgSoldier[i][j] = img;
		}

		sdState[i] = mtIdle;
	}
	delete img;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MidBossBehaveMax; j++)
		{
			ii = &imageInfo[j + 11];
			tex = createImage(ii->path, i);
			img = new iImage();
			img->addObject(tex);
			img->position = ii->p;
			img->scale = ii->s;
			freeImage(tex);

			imgTower[i][j] = img;
		}
		twState[i] = mtIdle;
	}
#if 0
	for (int i = 0; i < 3; i++)
	{
		tex = createImage(imageInfo[1].path, i);
		imgTower[i]->addObject(tex);
		imgTower[i]->position = imageInfo[1].p;
		imgTower[i]->scale = imageInfo[1].s;
		freeImage(tex);
	}
	iImage* img = new iImage();
	for (int i = 0; i < imageInfo[2].num; i++)
	{
		tex = createImage(iColor4fMake(63, 72, 204, 255), imageInfo[2].path, i +1);
		img->addObject(tex);
		freeImage(tex);		
	}
	iImage* _imgDoor = img;

	img = new iImage();
	for (int i = 0; i < imageInfo[5].num; i++)
	{
		tex = createImage(imageInfo[5].path, i + 1);
		img->addObject(tex);
		freeImage(tex);
	}
	iImage* _imgSoldier = img;

	EnemyImageInfo* ii;
	iImage** _imgCurtain = new iImage * [CurtainStateMax];
	for (int i = 0; i < CurtainStateMax; i++)
	{
		ii = &imageInfo[i + 3];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{
			tex = createImage(iColor4fMake(32, 8, 0, 255), ii->path, j + 1);
			img->addObject(tex);
			freeImage(tex);
		}
		_imgCurtain[i] = img;
	}

	img = new iImage();
	for (int i = 0; i < imageInfo[6].num; i++)
	{
		tex = createImage(iColor4fMake(255,0,0,255),
			imageInfo[6].path, i + 1);
		img->addObject(tex);
		freeImage(tex);
	}
	iImage* _imgMis = img;

	img = new iImage();
	for (int i = 0; i < imageInfo[7].num; i++)
	{
		tex = createImage(iColor4fMake(63, 72, 204, 255),
			imageInfo[7].path, i + 1);
		img->addObject(tex);
		freeImage(tex);
	}
	iImage* _imgTrail = img;
#endif
////////////////////////////////////////////////////////
//Initialize	
////////////////////////////////////////////////////////

#if 0
	for (int i = 0; i < 3; i++)
	{
		img = _imgDoor->clone();
		img->position = imageInfo[2].p;
		img->scale = imageInfo[2].s;
		img->_aniDt = imageInfo[2].aniDt;
		img->_repeatNum = imageInfo[2].repeatNum;
		imgDoor[i] = img;

		for (int j = 0; j < CurtainStateMax; j++)
		{
			img = _imgCurtain[j]->clone();
			img->position = imageInfo[j + 3].p;
			img->scale = imageInfo[j + 3].s;
			img->_aniDt = imageInfo[j + 3].aniDt;
			img->_repeatNum = imageInfo[j + 3].repeatNum;

			imgCurtain[i][j] = img;
		}

		ctState[i] = Idle;

		img = _imgSoldier->clone();
		img->position = imageInfo[5].p;
		img->scale = imageInfo[5].s;
		img->_aniDt = imageInfo[5].aniDt;
		img->_repeatNum = imageInfo[5].repeatNum;
		imgSoldier[i] = img;

		img = _imgMis->clone();
		img->position = imageInfo[6].p;
		img->position = imageInfo[6].p;
		img->scale = imageInfo[6].s;
		img->_aniDt = imageInfo[6].aniDt;
		img->_repeatNum = imageInfo[6].repeatNum;
		img->startAnimation();
		imgMis[i] = img;

		img = _imgTrail->clone();
		img->position = imageInfo[7].p;
		img->position = imageInfo[7].p;
		img->scale = imageInfo[7].s;
		img->_aniDt = imageInfo[7].aniDt;
		img->_repeatNum = imageInfo[7].repeatNum;
		img->startAnimation();
		imgTrail[i] = img;
	}

	delete _imgDoor;

	//delete imgCurtain;
#endif
	for (int i = 0; i < 2; i++)
	{		
		delete _imgBase[i];
		delete _imgAdd[i];
	}	
	delete _imgBase;
	delete _imgAdd;
}

bool flag;
void MoskTower::updateObj(float dt)
{
#if 1
	for (int i = 0; i < 3; i++)
	{
		flag = movePoint(towerP[i], towerP[i], iPointMake(towerP[i].x, p.y - 600), 1);
		setDoorPos(towerP[i]);

		if (flag)
		{
			if (imgDoor[i]->frame==0)
			{
				imgCurtain[i][state]->startAnimation(cbAniCurOpen, this);
				imgDoor[i]->startAnimation(cbAniDoorOpen, this);
			}
		}
		rate[i] += dt;
		if (rate[i] > _rate)
		{
			rate[i] -= _rate;			
			fire(dt);
		}
	}
#endif
		
}
void MoskTower::fixedUpdate(float dt)
{
}
void MoskTower::fire(float dt)
{
}
void MoskTower::dead()
{
	for (int i = 0; i < 3; i++)
	{
		sdState[i] = mtDead;
		ctState[i] = mtDead; 
		twState[i] = mtDead;
		state = mtDead;		

		imgDoor[i]->position.y += 300;
		for (int j = 0; j < 2; j++)
			imgDoor[i][j].position.y += 300;
	}	
}
void MoskTower::drawObj(float dt, iPoint off)
{
	if (isActive)
	{
		for (int i = 0; i < 3; i++)
		{
			imgTower[i][twState[i]]->paint(dt, off + p + towerP[i]);			
			imgSoldier[i][sdState[i]]->paint(dt, off + p + doorP[i]);			
			imgCurtain[i][ctState[i]]->paint(dt, off + p + doorP[i]);
			imgDoor[i]->paint(dt, off + p + doorP[i]);	
			//imgMis[i]->paint(dt, off + p + doorP[i]);
			//imgTrail[i]->paint(dt, off + p + doorP[i]);
		}
		imgBase[state]->paint(dt, off + p);

		for (int i = 0; i < 2; i++)
		{
			iPoint pos;
			if (i == 0)
				pos = iPointMake(p.x - 120, p.y - 10);
			else if (i == 1)
				pos = iPointMake(p.x + 230, p.y - 10);
			imgAdd[state]->paint(dt, off + pos);
		}
	}

#ifdef _DEBUG
	setRGBA(1, 0, 1, 1);
	drawDot(p + off);
	setRGBA(1, 1, 1, 1);
#endif // DEBUG

}

void MoskTower::freeeObj()
{
}


void MoskTower::cbAniDoorOpen(void* parm)
{
	MoskTower* mt = (MoskTower*)parm;

	for (int i = 0; i < 3; i++)
	{
		mt->imgDoor[i]->stopAnimation();
		mt->doorOpen[i] = true;
		mt->ctState[i] = mtDead;
		mt->imgCurtain[i][mt->ctState[i]]->startAnimation();
		mt->imgSoldier[i][mt->sdState[i]]->startAnimation();
		
	}
	printf("cbAni Door Open\n");
}

void MoskTower::cbAniCurOpen(void* parm)
{
	printf("Curtain was opened\n");
}

void MoskTower::cbAniAddOut(void* parm)
{
	MoskTower* mt = (MoskTower*)parm;

	for (int i = 0; i < 3; i++)
	{
		
	}
	mt->isActive = false;
}

ImageInfo imageInfo[] =
{
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Base.png",
		1, 2.f, { -370 / 2, -117},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Base_Destroyed.png",
		1, 2.f, { -369 / 2, -119},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Add.png",
		1, 2.f, { -38 / 2, -55},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Add_Destroyed_%02d.png",
		14, 2.f, { -151 / 2, -132},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Door_%02d.png",
		16, 2.f, { -90 / 2, -57},
		0.08f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Fire_%02d.png",
		30, 2.f, { -50 / 2, -26},
		0.08f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Trail_%02d.png",
		32, 2.f, { -62 / 2, -45},
		0.08f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Curtain_%02d.png",
		6, 2.f, { -48 / 2, -54},
		0.18f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Curtain_Open_%02d.png",
		13, 2.f, { -64 / 2, -59},
		0.05f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Soldier_%02d.png",
		8, 2.f, { -34 / 2, -41},
		0.08f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Soldier_Dead_%02d.png",
		7, 2.f, { -43 / 2, -44},
		0.1f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_%d.png",
		1, 2.f, { -86 / 2, -136},
		0.1f,
		0,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Destroyed_%d.png",
		1, 2.f, { -94 / 2, -102},
		0.08f,
		1,
		NULL,
	},
	{
		"Resources/Character/Enemies/MidBoss/MidBoss_Missiles_%02d.png",
		3, 2.f, { -94 / 2, -102},
		0.05f,
		1,
		NULL,
	},
};
#include "EnemyMgr.h"

//////////////////////////////////////////////
// extern func
//////////////////////////////////////////////
ProcEnemy*** _enemies;
ProcEnemy** enemies;

MoskTower* moskTower;
Masknell* maskNell;
int enemyCount;
void loadProcEnemy()
{
	_enemies = new ProcEnemy * *[(int)EnemyIndex::EnemyIndexMax];
	for (int i = 0; i < (int)EnemyIndex::EnemyIndexMax; i++)
	{
		_enemies[i] = new ProcEnemy * [enemyMax];
		if (i == 0)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
		else if (i == 1)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabBurserker(i);
		}
	}
	enemies = new ProcEnemy * [(int)EnemyIndex::EnemyIndexMax * enemyMax];
	enemyCount = 0;

	//addProcEnemy(EnemyIndex::ArMelee, iPointMake(350, 100));
	//addProcEnemy((int)EnemyIndex::ArBurserker, iPointMake(400, 100));
	//addProcEnemy(ArabCamel, iPointMake(200, 100));
	//addProcEnemy(ArabMelee, iPointMake(400, 100));
	//addProcEnemy(ArabMelee, iPointMake(450, 100));
	//addBoss((int)EnemyIndex::MTower, iPointMake(4080, 420));	
	addBoss((int)EnemyIndex::Masknell, iPointMake(4080, 420));
}

void freeProcEnemy()
{
	for (int i = 0; i < (int)EnemyIndex::EnemyIndexMax; i++)
	{
		for (int j = 0; j < enemyMax; j++)
		{
			_enemies[i][j]->releaseObj();
			delete _enemies[i][j];
		}
		delete _enemies[i];
	}
	delete _enemies;
	delete enemies;
}

void drawProcEnemy(float dt, iPoint off)
{
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];

		if (e->isActive)
		{
			if (e->getState() != (EnemyBehave)(eDeadL + (e->state % 2)))
				e->updateObj(dt);
			e->renderObj(dt, off);
			e->updateAi(e, dt);
		}

		if (e->isActive == false)
		{
			enemyCount--;
			enemies[i] = enemies[enemyCount];
			i--;
		}

	}
	//moskTower->updateObj(dt);
	//moskTower->renderObj(dt, off);
}

void addProcEnemy(int idx, iPoint p)
{	
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[idx][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->p = p;
			e->idx = (EnemyIndex)idx;
			e->initObj();
			enemies[enemyCount] = e;
			enemyCount++;
			return;
		}
	}
}

void addBoss(int idx, iPoint p)
{
	//load
	//moskTower = new MoskTower();
	//
	//moskTower->isActive = true;
	//moskTower->p = p;
	//moskTower->initObj();

	maskNell = new Masknell(idx);
	maskNell->isActive = true;
	maskNell->p = p;
	maskNell->initObj();
}

iImage** createImgChar(EnemyImageInfo* imageInfo, void* c)
{
	iImage** imgEnemy = new iImage * [imageInfo->num];
	for (int i = 0; i < imageInfo->num; i++)
	{
		iImage* img;
		img = new iImage();
		EnemyImageInfo* ei = &imageInfo[i / 2];		//left, right
		for (int j = 0; j < ei->num; j++)
		{
			Texture* tex = createImage(iColor4fMake(255, 0, 0, 255),
				ei->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->scale = ei->s;
		img->position = ei->p;

		if (ei->repeatNum)
			img->_repeatNum = ei->repeatNum;

		img->_aniDt = ei->aniDt;
		img->anc = TOP | LEFT;
		img->startAnimation(ei->cbAni, c);
		imgEnemy[i] = img;
	}

	return imgEnemy;
}

iImage** createImgCharReverse(EnemyImageInfo* imageInfo, void* c)
{
	int n = imageInfo->num * 2;

	iImage** imgEnemy = new iImage * [n];
	for (int i = 0; i < n; i++)
	{
		iImage* img;
		if (i % 2 == 1)
		{
			img = imgEnemy[i - 1]->clone();
			img->reverse = REVERSE_WIDTH;
		}
		else
		{
			img = new iImage();
			EnemyImageInfo* ei = &imageInfo[i / 2];		//left, right
			for (int k = 0; k < ei->num; k++)
			{
				Texture* tex = createImage(iColor4fMake(255, 0, 0, 255),
					ei->path, 1 + k);
				img->addObject(tex);
				freeImage(tex);
			}
			img->scale = ei->s;
			img->position = ei->p;

			if (ei->repeatNum)
				img->_repeatNum = ei->repeatNum;

			img->_aniDt = ei->aniDt;
			img->anc = TOP | LEFT;
			img->startAnimation(ei->cbAni, c);

		}
		imgEnemy[i] = img;
	}
	return imgEnemy;
}








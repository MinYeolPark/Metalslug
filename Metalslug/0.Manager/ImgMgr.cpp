#include "ImgMgr.h"

iImage** createImgSelect(Texture* bg)
{
	iImage** _img = new iImage * [CharacterIndexMax];
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 3; j++)
		{
			Texture* t = createImage("assets/CharSelect/CharacterSelect_%d%d.png", i, j);
			img->addObject(t);
			freeImage(t);

			t->width *= devSize.width / bg->width;
			t->potWidth *= devSize.width / bg->width;
			t->height *= devSize.height / bg->height;
			t->potHeight *= devSize.height / bg->height;

			img->position = iPointMake(30 * i + img->tex->width, 75);
		}
		_img[i] = img;
	}
	return _img;
}

iImage** createImgChar(ImageInfo* imageInfo, void* c)
{
	iImage** _img = new iImage * [imageInfo->num];
	for (int i = 0; i < imageInfo->num; i++)
	{
		iImage* img;
		img = new iImage();
		ImageInfo* ii = &imageInfo[i];
		for (int j = 0; j < ii->num; j++)
		{
			Texture* tex = createImage(iColor4bMake(255, 0, 0, 255),
				ii->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->scale = ii->s;
		img->position = ii->p;

		if (ii->repeatNum)
			img->_repeatNum = ii->repeatNum;

		img->_aniDt = ii->aniDt;
		img->anc = TOP | LEFT;
		img->startAnimation(ii->cbAni, c);
		_img[i] = img;
	}

	return _img;
}

iImage** createImgPlayer(ImageInfo* imageInfo, int size, void* c)
{
	iImage** _imgPlayer = new iImage * [size];
	memset(_imgPlayer, 0x00, sizeof(iImage*) * size);
	for (int i = 0; i < size; i++)
	{
		iImage* img;
		if (i % 2 == 1)
		{
			img = _imgPlayer[i - 1]->clone();
			img->reverse = REVERSE_WIDTH;
		}
		else
		{
			img = new iImage();
			ImageInfo* ii = &imageInfo[i / 2];		//left, right
			for (int k = 0; k < ii->num; k++)
			{
				Texture* tex = createImage(ii->colorKey, ii->path, k);
				img->addObject(tex);
				freeImage(tex);
			}
			img->scale = ii->s;
			img->position = ii->p;

			img->_aniDt = ii->aniDt;
			if (ii->repeatNum)
				img->_repeatNum = ii->repeatNum;
			if (ii->cbAni)
				img->startAnimation(ii->cbAni, c);
			else
				img->startAnimation();

		}
		_imgPlayer[i] = img;
	}
	return _imgPlayer;
}


iImage** createImgBullets(ImageInfo* imageInfo, void* b)
{
	ProcBullets* bul = (ProcBullets*)b;
	iImage** _imgs = new iImage * [BulletIndexMax];
	memset(_imgs, 0x00, sizeof(iImage*) * BulletIndexMax);
	for (int i = 0; i < BulletIndexMax; i++)
	{
		ImageInfo* ii = &imageInfo[i];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{
			Texture* tex = createImage(ii->colorKey, ii->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->scale = ii->s;
		img->position = ii->p;
		img->_aniDt = ii->aniDt;
		img->anc = TOP | LEFT;
		if (ii->repeatNum)
			img->_repeatNum = ii->repeatNum;
		_imgs[i]=img;
	}
	return _imgs;
}

iImage** createImgEffect(ImageInfo* imageInfo, void* c)
{	
	iImage** _imgs = new iImage * [EffectIndexMax];
	memset(_imgs, 0x00, sizeof(iImage*) * EffectIndexMax);
	for (int i = 0; i < EffectIndexMax; i++)
	{
		ImageInfo* ii = &imageInfo[i];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{
			Texture* tex = createImage(ii->colorKey, ii->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->scale = ii->s;
		img->position = ii->p;
		img->_aniDt = ii->aniDt;
		img->anc = TOP | LEFT;		
		if (ii->repeatNum)
			img->_repeatNum = ii->repeatNum;
		_imgs[i] = img;
	}
	return _imgs;
}

iImage** createImgEnemy(ImageInfo* imageInfo, void* c)
{
	return nullptr;
}

iImage** createImgReverse(ImageInfo* imageInfo, int size, void* c)
{
	iImage** _imgs = new iImage * [size * 2];
	//memset(_imgs, 0x00, sizeof(iImage*) * size * 2);
	for (int i = 0; i < size; i++)
	{
		iImage* img;
		if (i % 2 == 1)
		{
			img = _imgs[i - 1]->clone();
			img->reverse = REVERSE_WIDTH;
		}
		else
		{
			img = new iImage();
			ImageInfo* ii = &imageInfo[i / 2];		//left, right
			for (int k = 0; k < ii->num; k++)
			{
				Texture* tex = createImage(ii->colorKey, ii->path, k);
				img->addObject(tex);
				freeImage(tex);
			}
			img->scale = ii->s;
			img->position = ii->p;

			img->_aniDt = ii->aniDt;
			if (ii->repeatNum)
				img->_repeatNum = ii->repeatNum;

			img->anc = TOP | LEFT;
			if (ii->cbAni)
				img->startAnimation(ii->cbAni, c);
			else
				img->startAnimation();

		}
		_imgs[i] = img;
	}
	return _imgs;
}

iImage** createImgSingleBehave(ImageInfo* imageInfo, void* c)
{
	return nullptr;
}

iImage** createImgSingleEach(ImageInfo* imageInfo, void* c)
{
	return nullptr;
}

iImage** createSingleImage(ImageInfo* imageInfo, int size, void* c)
{	
	iImage** _imgs = new iImage * [size];
	memset(_imgs, 0x00, sizeof(iImage*) * size);
	for (int i = 0; i < size; i++)
	{
		ImageInfo* ii = &imageInfo[i];
		iImage* img = new iImage();
		for (int j = 0; j < ii->num; j++)
		{
			Texture* tex = createImage(ii->colorKey, ii->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->scale = ii->s;
		img->position = ii->p;
		img->_aniDt = ii->aniDt;
		img->anc = TOP | LEFT;
		img->startAnimation(ii->cbAni, c);
		if (ii->repeatNum)
			img->_repeatNum = ii->repeatNum;
		_imgs[i] = img;
	}
	return _imgs;
}

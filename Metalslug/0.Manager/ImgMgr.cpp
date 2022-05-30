#include "ImgMgr.h"

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
			Texture* tex = createImage(iColor4fMake(255, 0, 0, 255),
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

iImage** createImgCharReverse(ImageInfo* imageInfo, void* c)
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
			ImageInfo* ii = &imageInfo[i / 2];		//left, right
			for (int k = 0; k < ii->num; k++)
			{
				Texture* tex = createImage(iColor4fMake(255, 0, 0, 255),
					ii->path, 1 + k);
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

		}
		imgEnemy[i] = img;
	}
	return imgEnemy;
}

iImage** createImgBullets(ImageInfo* imageInfo, void* c)
{
	iImage** _img;
	for (int i = 0; i < BulletIndexMax; i++)
	{
		ImageInfo* ii = &imageInfo[i];
		_img = new iImage * [imageInfo->num];
		for (int j = 0; j < imageInfo->num; j++)
		{
			iImage* img = new iImage();
			for (int k = 0; k < ii->num; k++)
			{
				Texture* tex = createImage(iColor4fMake(255,255,255,255),
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
	}
	return _img;
}

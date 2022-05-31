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

iImage** createImgPlayer(ImageInfo* imageInfo, void* c)
{
	iImage** _imgPlayer = new iImage * [PlayerBehaveMax];
	memset(_imgPlayer, 0x00, sizeof(iImage * [PlayerBehaveMax]));
	for (int i = 0; i < PlayerBehaveMax; i++)
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
				Texture* tex = createImage(iColor4bMake(255, 0, 0, 255),
					ii->path, k);
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
		_imgPlayer[i] = img;
	}
	return _imgPlayer;
}


iImage** createImgBullets(ImageInfo* imageInfo, void* c)
{
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
		img->startAnimation(ii->cbAni, (ProcBullets*)c);
		if (ii->repeatNum)
			img->_repeatNum = ii->repeatNum;
		_imgs[i]=img;
	}
	return _imgs;
}

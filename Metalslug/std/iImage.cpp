#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	imgs = new iArray(cb);
	tex = NULL;
	position = iPointZero;
	frame = 0;
	aniDt = 0.0f;
	_aniDt = default_aniDt;
	animation = false;
	repeatNum = _repeatNum = 0;// inf
	method = NULL;
	parm = NULL;

	alpha = 1.0f;
	degree = 0.0f;
	scale = 1.0f;
	reverse = REVERSE_NONE;
	anc = TOP | LEFT;
}

iImage::~iImage()
{
	delete imgs;
}

void iImage::cb(void* data)
{
	freeImage((Texture*)data);
}

void iImage::paint(float dt, iPoint p)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt >= _aniDt)
		{
			aniDt -= _aniDt;

			frame++;
			if (frame > imgs->count - 1)
			{
				if (_repeatNum == 0)
					frame = 0;
				else// if (_repeatNum)
				{
					printf("repeatNum = %d\n", repeatNum);
					repeatNum++;
					if (repeatNum < _repeatNum)
						frame = 0;
					else
					{
						frame = imgs->count - 1;// frame--;
						animation = false;
						if (method)
							method(parm);
					}
				}
			}
		}
	}

	float r, g, b, a;
	getRGBA(r, g, b, a);
	setRGBA(r, g, b, a * alpha);

	tex = (Texture*)imgs->objectAtIndex(frame);

	float& x = p.x;
	float& y = p.y;
	float dx = tex->width;
	float dy = tex->height;

	switch (anc) {
	case TOP | LEFT:							break;
	case TOP | RIGHT:		x -= dx;			break;
	case TOP | HCENTER:		x -= dx / 2;		break;
	case BOTTOM | LEFT:					 y -= dy; break;
	case BOTTOM | RIGHT:	x -= dx;	 y -= dy; break;
	case BOTTOM | HCENTER:	x -= dx / 2; y -= dy; break;
	case VCENTER | LEFT:				 y -= dy / 2; break;
	case VCENTER | RIGHT:	x -= dx;	 y -= dy / 2; break;
	case VCENTER | HCENTER:	x -= dx / 2; y -= dy / 2; break;
	}

	if (reverse & REVERSE_WIDTH)
		x -= (position.x + dx) * scale;
	else
		x += position.x * scale;

	if (reverse && REVERSE_HEIGHT)
		y -= (position.y + dy) * scale;
	else
		y += position.y * scale;
	anc = TOP | LEFT;

	drawImage(tex, p.x, p.y, scale, scale, anc,
		0, 0, tex->width, tex->height,
		2, degree, reverse);
	setRGBA(r, g, b, a);
}

void iImage::paint(float dt, iPoint p, iImageType type)
{
	if (type == iImageTypePop)
	{
		if (animation)
		{
			aniDt += dt;
			if (aniDt >= _aniDt)
			{
				aniDt -= _aniDt;

				frame++;
				if (frame > imgs->count - 1)
				{
					if (_repeatNum == 0)
						frame = 0;
					else// if (_repeatNum)
					{
						printf("repeatNum = %d\n", repeatNum);
						repeatNum++;
						if (repeatNum < _repeatNum)
							frame = 0;
						else
						{
							frame = imgs->count - 1;// frame--;
							animation = false;
							if (method)
								method(parm);
						}
					}
				}
			}
		}

		float r, g, b, a;
		getRGBA(r, g, b, a);
		setRGBA(r, g, b, a * alpha);

		tex = (Texture*)imgs->objectAtIndex(frame);
		p += position;

		p.x += (1.0f - scale) / 2 * tex->width;
		p.y += (1.0f - scale) / 2 * tex->height;
		drawImage(tex, p.x, p.y, scale, scale, anc,
			0, 0, tex->width, tex->height,
			2, degree, reverse);
		setRGBA(r, g, b, a);
	}
}

void iImage::addObject(Texture* tex)
{
	if (this->tex == NULL)
		this->tex = tex;
	imgs->addObject(tex);
	tex->retainCount++;
}
void iImage::startAnimation(cbAnimation m, void* p)
{
	animation = true;
	frame = 0;
	aniDt = 0.0f;
	method = m;
	parm = p;

	if (_repeatNum)
		repeatNum = 0;
}
void iImage::stopAnimation()
{
	animation = false;
}

iRect iImage::rect(iPoint p, iSize s)
{
	return iRectMake(
		position.x + p.x - s.width / 2,
		position.y + p.y - s.height / 2,
		tex->width + s.width,
		tex->height + s.height);
}

iPoint iImage::center(iPoint p)
{
	iRect rt = rect(p);
	return iPointMake(	rt.origin.x + rt.size.width / 2,
						rt.origin.y + rt.size.height / 2);
}

iImage* iImage::clone()
{
	iImage* img = new iImage();

	memcpy(img, this, sizeof(iImage));

	img->imgs = new iArray(cb);
	for (int i = 0; i < imgs->count; i++)
		img->addObject( (Texture*)imgs->objectAtIndex(i) );

	return img;
}
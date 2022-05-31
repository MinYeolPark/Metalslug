#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include <gl/glew.h>
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.lib")

#include "iCommon.h"
#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iString.h"

#include "iMatrix.h"
#include "iArray.h"
#include "iImage.h"
#include "iPopup.h"
#include "iGraphics.h"		//iSize, iColor
#include "iStrTex.h"

#include "iUtil.h"

#include "iOpenGL.h"

#define va_start_end(szFormat, strText)  {					\
	va_list args;											\
	va_start(args, szFormat);								\
	vsnprintf(strText, sizeof(strText), szFormat, args);	\
	va_end(args);											\
}

//640 : 480 = x : 264
#define DEV_WIDTH 352
#define DEV_HEIGHT 264
extern iSize devSize;
extern iRect viewport;

void setZoom(iPoint c, float scale, float zoomDt, float delayDt);
void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);
void clearScreen();

void setClip(int x, int y, int w, int h);

void setDotSize(float size);
float getDotSize();
void drawDot(iPoint p);
void drawDot(float x, float y);

void setLineWidth(float width);
float getLineWidth();
void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(iRect rt, float radius = 0.0f, float degree = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f, float degree = 0.0f);
void fillRect(iRect rt, float radius = 0.0f, float degree = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f, float degree = 0.0f);
struct Texture
{
	uint32 texID;
	float width, height;
	uint32 potWidth, potHeight;
	int retainCount;
};


uint32 nextPOT(uint32 x);
void freeImage(Texture* tex);
void drawImage(Texture* tex, iPoint p, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc,// dst
	int ix, int iy, int iw, int ih,// src
	int xyz, int degree, int reverse = REVERSE_NONE);
Texture* createImageWithRGBA(int width, int height, uint8* rgba, iColor4b* colorKey = NULL);
Texture* createImage(const char* szFormat, ...);
Texture* createImage(iColor4b colorKey, const char* szFormat, ...);
Texture** createImageDivide(int numX, int numY, const char* szFormat, ...);

void setStringName(const char* stringName);
const char* getStringName();
void setStringSize(float size);
float getStringSize();
void setStringBorder(float width);
float getStringBorder();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
iRect rectOfString(const char* szFormat, ...);
void drawString(float x, float y, int anc, const char* szFormat, ...);

Texture* createTexture(int width, int height);
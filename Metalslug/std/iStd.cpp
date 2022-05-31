#include "iStd.h"

#include "iWindow.h"

iSize devSize;
iRect viewport;
static float cr, cg, cb, ca;
void setRGBA(float r, float g, float b, float a)
{
	cr = r;
	cg = g;
	cb = b;
	ca = a;
}
void getRGBA(float& r, float& g, float& b, float& a)
{
	r = cr;
	g = cg;
	b = cb;
	a = ca;
}
void clearScreen()
{
	iFBO::clear(cr, cg, cb, ca);
}

void setClip(int x, int y, int w, int h)
{
	if (x == 0 && y == 0 && w == 0 && h == 0)
	{
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{	// glViewport ����Ϳ����� ������
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, devSize.height - y - h, w, h);
	}
}

static float dotSize = 1.0f;
void setDotSize(float size)
{
	dotSize = size;
}
float getDotSize()
{
	return dotSize;
}
void drawDot(float x, float y)
{
	drawDot(iPointMake(x, y));
}
void drawDot(iPoint p)
{
	static GLuint programID = 0;
	if (programID == 0)
	{
		programID = createProgramID("assets/shader/dotVert.glsl",
			"assets/shader/dotFrag.glsl");
	}
	glUseProgram(programID);

	float r = dotSize / 2;
	float position[16] = {
		-r, -r, 0, 1,		+r, -r, 0, 1,
		-r, +r, 0, 1,		+r, +r, 0, 1,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), position);

	GLuint attrPostion = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(attrPostion);
	glVertexAttribPointer(attrPostion, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

	matrixModelview->loadIdentity();
	matrixModelview->translate(p.x, p.y, 0);

	GLint uid = glGetUniformLocation(programID, "mProject");
	glUniformMatrix4fv(uid, 1, false, matrixProject->d());
	uid = glGetUniformLocation(programID, "mModelview");
	glUniformMatrix4fv(uid, 1, false, matrixModelview->d());

	uid = glGetUniformLocation(programID, "center");
	glUniform2f(uid, p.x, devSize.height - p.y);

	uid = glGetUniformLocation(programID, "dotSize");
	glUniform1f(uid, dotSize);

	uid = glGetUniformLocation(programID, "color");
	glUniform4f(uid, cr, cg, cb, ca);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(attrPostion);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
static float lineWidth = 1.0f;
void setLineWidth(float width)
{
	lineWidth = width;
}
float getLineWidth()
{
	return lineWidth;
}
void drawLine(float x0, float y0, float x1, float y1)
{
	drawLine(iPointMake(x0, y0), iPointMake(x1, y1));
}
void drawLine(iPoint sp, iPoint ep)
{
	static GLuint programID = 0;
	if (programID == 0)
	{
		programID = createProgramID("assets/shader/lineVert.glsl",
			"assets/shader/lineFrag.glsl");
	}
	glUseProgram(programID);

	iPoint c = (sp + ep) / 2;
	float w = iPointLength(sp - c);
	float h = (int)(lineWidth / 2 + 0.5f);
	float position[16] = {
		-w - h, -h, 0, 1,		+w + h, -h, 0, 1,
		-w - h, +h, 0, 1,		+w + h, +h, 0, 1,
	};
	float degree = -iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), position);

	GLuint attrPostion = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(attrPostion);
	glVertexAttribPointer(attrPostion, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

	matrixModelview->loadIdentity();
	matrixModelview->translate(c.x, c.y, 0);
	matrixModelview->rotate(0, 0, 1, degree);

	GLint uid = glGetUniformLocation(programID, "mProject");
	glUniformMatrix4fv(uid, 1, false, matrixProject->d());
	uid = glGetUniformLocation(programID, "mModelview");
	glUniformMatrix4fv(uid, 1, false, matrixModelview->d());

	uid = glGetUniformLocation(programID, "sp");
	glUniform2f(uid, sp.x, devSize.height - sp.y);

	uid = glGetUniformLocation(programID, "ep");
	glUniform2f(uid, ep.x, devSize.height - ep.y);

	uid = glGetUniformLocation(programID, "lineWidth");
	glUniform1f(uid, lineWidth);

	uid = glGetUniformLocation(programID, "color");
	glUniform4f(uid, cr, cg, cb, ca);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(attrPostion);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawRect(iRect rt, float radius, float degree)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius, degree);
}
void drawRect(float x, float y, float width, float height, float radius, float degree)
{
	static GLuint programID = 0;
	if (programID == 0)
	{
		programID = createProgramID("assets/shader/drawRectVert.glsl",
			"assets/shader/drawRectFrag.glsl");
	}
	glUseProgram(programID);

	float position[16] = {
		-width / 2 - 1, -height / 2 - 1, 0, 1,		+width / 2 + 1, -height / 2 - 1, 0, 1,
		-width / 2 - 1, +height / 2 + 1, 0, 1,		+width / 2 + 1, +height / 2 + 1, 0, 1,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), position);

	GLuint attrPostion = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(attrPostion);
	glVertexAttribPointer(attrPostion, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

	matrixModelview->loadIdentity();
	matrixModelview->translate(x + width / 2, y + height / 2, 0);
	matrixModelview->rotate(0, 0, 1, degree);

	GLint uid = glGetUniformLocation(programID, "mProject");
	glUniformMatrix4fv(uid, 1, false, matrixProject->d());
	uid = glGetUniformLocation(programID, "mModelview");
	glUniformMatrix4fv(uid, 1, false, matrixModelview->d());

	uid = glGetUniformLocation(programID, "u_position");
	glUniform2f(uid, x + width / 2, devSize.height - y - height / 2);

	uid = glGetUniformLocation(programID, "u_size");
	glUniform2f(uid, width / 2, height / 2);

	uid = glGetUniformLocation(programID, "u_radius");
	glUniform1f(uid, radius);

	uid = glGetUniformLocation(programID, "u_lineWidth");
	glUniform1f(uid, lineWidth);

	uid = glGetUniformLocation(programID, "u_radian");
	glUniform1f(uid, degree * M_PI / 180);

	uid = glGetUniformLocation(programID, "u_color");
	glUniform4f(uid, cr, cg, cb, ca);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(attrPostion);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void fillRect(iRect rt, float radius, float degree)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius, degree);
}
void fillRect(float x, float y, float width, float height, float radius, float degree)
{
	static GLuint programID = 0;
	if (programID == 0)
	{
		programID = createProgramID("assets/shader/fillRectVert.glsl",
			"assets/shader/fillRectFrag.glsl");
	}
	glUseProgram(programID);

	float position[16] = {
		-width / 2 - 1, -height / 2 - 1, 0, 1,		+width / 2 + 1, -height / 2 - 1, 0, 1,
		-width / 2 - 1, +height / 2 + 1, 0, 1,		+width / 2 + 1, +height / 2 + 1, 0, 1,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), position);

	GLuint attrPostion = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(attrPostion);
	glVertexAttribPointer(attrPostion, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

	matrixModelview->loadIdentity();
	matrixModelview->translate(x + width / 2, y + height / 2, 0);
	matrixModelview->rotate(0, 0, 1, degree);

	GLint uid = glGetUniformLocation(programID, "mProject");
	glUniformMatrix4fv(uid, 1, false, matrixProject->d());
	uid = glGetUniformLocation(programID, "mModelview");
	glUniformMatrix4fv(uid, 1, false, matrixModelview->d());

	uid = glGetUniformLocation(programID, "u_position");
	glUniform2f(uid, x + width / 2, devSize.height - y - height / 2);

	uid = glGetUniformLocation(programID, "u_size");
	glUniform2f(uid, width / 2, height / 2);

	uid = glGetUniformLocation(programID, "u_radius");
	glUniform1f(uid, radius);

	uid = glGetUniformLocation(programID, "u_radian");
	glUniform1f(uid, degree * M_PI / 180);

	uid = glGetUniformLocation(programID, "u_color");
	glUniform4f(uid, cr, cg, cb, ca);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(attrPostion);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int texNum = 0;
uint32 nextPOT(uint32 x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1; int texNum = 0;

}
void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	texNum--;
	glDeleteTextures(1, &tex->texID);
	delete tex;
}
void drawImage(Texture* tex, iPoint p, int anc)
{
	drawImage(tex, p.x, p.y, 1.0f, 1.0f, anc, 0, 0, tex->width, tex->height, 2, 0);
}
void drawImage(Texture* tex, float x, float y, int anc)
{
	drawImage(tex, x, y, 1.0f, 1.0f, anc, 0, 0, tex->width, tex->height, 2, 0);
}
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc)
{
	drawImage(tex, x, y, ratX, ratY, anc, 0, 0, tex->width, tex->height, 2, 0);
}
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc,// dst
	int ix, int iy, int iw, int ih,// src
	int xyz, int degree, int reverse)
{
	float dx = iw * ratX;
	float dy = ih * ratY;

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
#if 0
	RectF dstRect = { x, y, dx, dy };
	RectF srcRect = { ix, iy, iw, ih };
	graphics->DrawImage(img, dstRect, srcRect, UnitPixel);
#endif

	iPoint p[4] = {
		{x, y},				{x + dx, y},
		{x, y + dy},		{x + dx, y + dy},
	};

	if (reverse & REVERSE_WIDTH)
	{
		float t = p[0].x;
		p[0].x = p[1].x;
		p[1].x = t;

		p[2].x = p[0].x;
		p[3].x = p[1].x;
	}
	if (reverse & REVERSE_HEIGHT)
	{
		float t = p[0].y;
		p[0].y = p[2].y;
		p[2].y = t;

		p[1].y = p[0].y;
		p[3].y = p[2].y;
	}
	iColor4f c[4] = {
		{cr, cg, cb, ca},				{cr, cg, cb, ca},
		{cr, cg, cb, ca},				{cr, cg, cb, ca},
	};
	iPoint st[4] = {
		{0, 0},							{tex->width / tex->potWidth, 0},
		{0, tex->height / tex->potHeight},{tex->width / tex->potWidth, tex->height / tex->potHeight},
	};

	matrixModelview->loadIdentity();
	if (degree)
	{
		p[0] = iPointMake(-dx / 2, -dy / 2);	p[1] = iPointMake(+dx / 2, -dy / 2);
		p[2] = iPointMake(-dx / 2, +dy / 2);	p[3] = iPointMake(+dx / 2, +dy / 2);

		matrixModelview->translate(x + dx / 2, y + dy / 2, 0);
		matrixModelview->rotate(xyz == 0, xyz == 1, xyz == 2, degree);
	}

	Vertex vtx[4];
	for (int i = 0; i < 4; i++)
	{
		Vertex* v = &vtx[i];
		memcpy(v->position, &p[i], sizeof(iPoint));
		v->position[2] = 0;
		v->position[3] = 1;
		memcpy(v->color, &c[i], sizeof(iColor4f));
		memcpy(v->uv, &st[i], sizeof(iPoint));
	}

	static GLuint programID = 0;
	if (programID == 0)
	{
		programID = createProgramID("assets/shader/alphaVert.glsl",
			"assets/shader/alphaFrag.glsl");
	}
	glUseProgram(programID);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), vtx);

	GLuint attrPostion = glGetAttribLocation(programID, "position");
	GLuint attrColor = glGetAttribLocation(programID, "color");
	GLuint attrSt = glGetAttribLocation(programID, "st");
	glEnableVertexAttribArray(attrPostion);
	glEnableVertexAttribArray(attrColor);
	glEnableVertexAttribArray(attrSt);
	glVertexAttribPointer(attrPostion, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
	glVertexAttribPointer(attrColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 4));
	glVertexAttribPointer(attrSt, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 8));

	glActiveTexture(GL_TEXTURE0);
	GLint uid = glGetUniformLocation(programID, "tex");
	glUniform1i(uid, 0);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	uid = glGetUniformLocation(programID, "mProject");
	glUniformMatrix4fv(uid, 1, false, matrixProject->d());
	uid = glGetUniformLocation(programID, "mModelview");
	glUniformMatrix4fv(uid, 1, false, matrixModelview->d());

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(attrPostion);
	glDisableVertexAttribArray(attrColor);
	glDisableVertexAttribArray(attrSt);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Texture* createImageWithRGBA(int width, int height, uint8* rgba, iColor4b* colorKey)
{
	int potWidth = nextPOT(width);
	int potHeight = nextPOT(height);

	if (colorKey != NULL)
	{
		int i, num = potWidth * potHeight;
		for (i = 0; i < num; i++)
		{
			uint8* c = &rgba[4 * i];
			if (c[0] == colorKey->r &&
				c[1] == colorKey->g &&
				c[2] == colorKey->b)
				c[3] = 0;
		}
	}
	uint32 texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	applyTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = potWidth;
	tex->potHeight = potHeight;
	tex->retainCount = 1;

	texNum++;

	return tex;
}

uint8* createImageData(uint32& width, uint32& height, const char* szFormat)
{
	wchar_t* ws = utf8_to_utf16(szFormat);
	Bitmap* bmp = Bitmap::FromFile(ws);
	delete ws;

	uint8* rgba = iGraphics::bmp2rgba(bmp, width, height);
	delete bmp;

	return rgba;
}

Texture* createImage(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	uint32 width, height;
	uint8* rgba = createImageData(width, height, szText);

	Texture* tex = createImageWithRGBA(width, height, rgba, NULL);
	delete rgba;

	return tex;
}

Texture* createImage(iColor4b colorKey, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	uint32 width, height;
	uint8* rgba = createImageData(width, height, szText);

	Texture* tex = createImageWithRGBA(width, height, rgba, &colorKey);
	delete rgba;

	return tex;
}

Texture** createImageDivide(int numX, int numY, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws);
	delete ws;

	uint32 width, height;
	uint8* rgba = iGraphics::bmp2rgba(bmp, width, height);//////////////////////////
	delete bmp;

	Texture** texs = new Texture * [numX * numY];
	int w = width / numX;
	int h = height / numY;
	int pw = nextPOT(w);
	int ph = nextPOT(h);
	uint8* newRGBA = new uint8[pw * ph * 4];
	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			//uint8* newRGBA = new uint8[pw * ph * 4];
			memset(newRGBA, 0x00, sizeof(uint8) * pw * ph * 4);
			for (int n = 0; n < h; n++)
			{
				for (int m = 0; m < w; m++)
				{
					uint8* dst = &newRGBA[pw * 4 * n + 4 * m];
					uint8* src = &rgba[width * 4 * (h * j + n) + 4 * (w * i + m)];
					memcpy(dst, src, sizeof(uint8) * 4);
				}
			}
			texs[numX * j + i] = createImageWithRGBA(w, h, newRGBA);
		}
	}
	delete newRGBA;
	delete rgba;//////////////////////////

	return texs;
}

static char* stringName = NULL;
static float stringSize, stringBorder;
static iColor4f stringColor, stringBorderColor;
void setStringName(const char* sn)
{
	if (stringName)
	{
		if (strcmp(stringName, sn) == 0)
			return;
		delete stringName;
	}
	int len = strlen(sn);
	stringName = new char[len + 1];
	strcpy(stringName, sn);
}
const char* getStringName()
{
	return stringName;
}

void setStringSize(float size)
{
	stringSize = size;
}
float getStringSize()
{
	return stringSize;
}
void setStringBorder(float width)
{
	stringBorder = width;
}
float getStringBorder()
{
	return stringBorder;
}
void setStringRGBA(float r, float g, float b, float a)
{
	stringColor = iColor4fMake(r, g, b, a);
}
void getStringRGBA(float& r, float& g, float& b, float& a)
{
	r = stringColor.r;
	g = stringColor.g;
	b = stringColor.b;
	a = stringColor.a;
}
void setStringBorderRGBA(float r, float g, float b, float a)
{
	stringBorderColor = iColor4fMake(r, g, b, a);
}
void getStringBorderRGBA(float& r, float& g, float& b, float& a)
{
	r = stringBorderColor.r;
	g = stringBorderColor.g;
	b = stringBorderColor.b;
	a = stringBorderColor.a;
}

iRect rectOfString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);
	return iGraphics::rectOfString(szText);
}

char* str = NULL;
Texture* texString = NULL;

void drawString(float x, float y, int anc, const char* szFormat, ...)
{
#if 0
	//#error Don't Use this Function until next update ...
	return;
#endif
	char szText[512];
	va_start_end(szFormat, szText);

	if (str == NULL || strcmp(str, szText))
	{
		iRect rt = rectOfString(szText);

		iGraphics* g = new iGraphics();
		g->init(rt.size);

		g->drawString(0, 0, TOP | LEFT, szText);

		if (str)
			delete str;
		str = new char[strlen(szText) + 1];
		strcpy(str, szText);

		if (texString)
			freeImage(texString);
		texString = g->getTexture();
		delete g;
	}

	drawImage(texString, x, y, anc);
}

Texture* createTexture(int width, int height)
{
	uint32 texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	applyTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = width;
	tex->potHeight = height;
	tex->retainCount = 1;

	texNum++;

	return tex;
}




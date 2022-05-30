#include "iPoint.h"

#include "iStd.h"
iPoint::iPoint()
{
	x = 0;
	y = 0;
}

iPoint::iPoint(const iPoint& p)
{
	x = p.x;
	y = p.y;
}

iPoint::iPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool iPoint::operator == (const iPoint& p)
{
	return x == p.x && y == p.y;
}

bool iPoint::operator != (const iPoint& p)
{
	return x != p.x || y != p.y;
}

iPoint& iPoint::operator = (const iPoint& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

//iPoint& iPoint::operator = (const iSize& s)
//{
//	x = s.width;
//	y = s.height;
//	return *this;
//}


iPoint iPoint::operator + (const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}
iPoint& iPoint::operator += (const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint iPoint::operator - (const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}
iPoint& iPoint::operator -= (const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

iPoint iPoint::operator * (float s)
{
	iPoint r;
	r.x = x * s;
	r.y = y * s;
	return r;
}
iPoint& iPoint::operator *= (float s)
{
	x *= s;
	y *= s;
	return *this;
}

iPoint iPoint::operator / (float s)
{
	iPoint r;
	r.x = x / s;
	r.y = y / s;
	return r;
}
iPoint& iPoint::operator /= (float s)
{
	x /= s;
	y /= s;
	return *this;
}

float iPoint::length()
{
	return sqrtf(x * x + y * y);
}

iPoint iPointMake(float x, float y)
{
	iPoint r;
	r.x = x;
	r.y = y;
	return r;
}

float iPointLength(iPoint p)
{
	return sqrtf(p.x * p.x + p.y * p.y);
}

iPoint iPointRotate(iPoint p, iPoint t, float degree)
{
	while (degree < 0) degree += 360;
	while (degree > 360) degree -= 360;
	float sinr = (float)_sin(degree);
	float cosr = (float)_cos(degree);

	iPoint r;
	r.x = t.x + (p.x - t.x)*cosr - (p.y - t.y)*sinr;
	r.y = t.y + (p.x - t.x)*sinr + (p.y - t.y)*cosr;

	return r;
}

float iPointAngle(const iPoint& v1, const iPoint& v2, const iPoint& v3)
{
#if 1// 제 2 코사인법칙(a2 = b2 + c2 - 2bc cosA)
	// v13^2 = v12^2 + v23^2 - 2 * v12 * v23 * cosA
	// A = acos( (b^2 + c^2 - a^2) / (2ab) );
	float a2 = (v1.x - v3.x) * (v1.x - v3.x) + (v1.y - v3.y) * (v1.y - v3.y);
	float b2 = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
	float c2 = (v2.x - v3.x) * (v2.x - v3.x) + (v2.y - v3.y) * (v2.y - v3.y);
	float degree = (float)acos((b2 + c2 - a2) / sqrtf(4 * b2 * c2)) * 180 / (float)M_PI;
	if (v3.y > v1.y)
		degree = 360 - degree;
	return degree;
#else// 내적(aXb = |a||b|cosA)
	iPoint a = iPointMake(v1.x - v2.x, v1.y - v2.y);
	iPoint b = iPointMake(v3.x - v2.x, v3.y - v2.y);
	float degree = acos((a.x * b.x + a.y * b.y) / sqrtf((a.x * a.x + a.y * a.y) * (b.x * b.x + b.y * b.y)))
		* 180 / M_PI;
	if (v3.y > v1.y)
		degree = 360 - degree;
	return degree;
#endif
}

float setDegree(float& degree, iPoint att, iPoint def, float spin, float dt)
{
	float d = 360 - iPointAngle(iPointMake(1, 0), iPointZero, def - att);
	float diff = d - degree;
	if (diff < 0) diff += 360;
	if (diff > 180) diff -= 360;

	float tDegree = degree + diff;
	if (degree < tDegree)
	{
		degree += 360 * dt;
		if (degree > tDegree)
			degree = tDegree;
	}
	else if (degree > tDegree)
	{
		degree -= 360 * dt;
		if (degree < tDegree)
			degree = tDegree;
	}

	return diff;
}

float linear(float r, float s, float e)
{
	return s + (e - s) * r;
}

iPoint linear(float r, iPoint s, iPoint e)
{
	return s + (e - s) * r;
}

float easeIn(float r, float s, float e)
{
	//return s + (e - s) * _sin(90 * r);
	return s + (e - s) * _sin( 90 * _sin(90 * r) );
}

iPoint easeIn(float r, iPoint s, iPoint e)
{
	//return s + (e - s) * _sin(90 * r);
	return s + (e - s) * _sin(90 * _sin(90 * r));
}

float easeOut(float r, float s, float e)
{
	//return s + (e - s) * (1.0f - _cos(90 * r));
	return s + (e - s) * (r * r * r);
}

iPoint easeOut(float r, iPoint s, iPoint e)
{
	//return s + (e - s) * (1.0f - _cos(90 * r));
	return s + (e - s) * (r * r * r);
}

float easeInOut(float r, float s, float e)
{
	if (r < 0.5f)
		return easeIn(r * 2, s, s + (e - s) / 2);
	return easeOut((r - 0.5f) * 2, s + (e - s) / 2, e);
}

iPoint easeInOut(float r, iPoint s, iPoint e)
{
	if (r < 0.5f)
		return easeIn(r * 2, s, s + (e - s) / 2);
	return easeOut((r - 0.5f) * 2, s + (e - s) / 2, e);
}

bool movePoint(iPoint& p, iPoint sp, iPoint ep, float speedDt)
{
	iPoint v = (ep - sp);
	v /= iPointLength(v);

	//p += v * (_speed * dt);
	v *= speedDt;

	if (p.x < ep.x)
	{
		p.x += v.x;
		if (p.x > ep.x)
			p.x = ep.x;
	}
	else if (p.x > ep.x)
	{
		p.x += v.x;
		if (p.x < ep.x)
			p.x = ep.x;
	}

	if (p.y < ep.y)
	{
		p.y += v.y;
		if (p.y > ep.y)
			p.y = ep.y;
	}
	else if (p.y > ep.y)
	{
		p.y += v.y;
		if (p.y < ep.y)
			p.y = ep.y;
	}

	return p == ep;
}
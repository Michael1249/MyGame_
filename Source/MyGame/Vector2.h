#pragma once

#include <math.h>
#define _PI_ 3.14159265358979f
template<typename value_t>
class Vector2
{
public:
	value_t x;
	value_t y;

	Vector2() :x(0), y(0) {};
	Vector2(value_t xValue, value_t yValue) :x(xValue), y(yValue) {};
	
	void Set(value_t xValue, value_t yValue) { x = xValue; y = yValue; };

	value_t Length() const { return sqrt(x * x + y * y); };
	value_t Length2() const { return x * x + y * y; };
	value_t Distance(const Vector2 & v) const { return sqrt(((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y))); };
	value_t Distance2(const Vector2 & v) const { return ((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y)); };
	value_t AngleRad() const { return atan2(y, x); }
	value_t AngleNormalized() const { return atan2(y, x)/(2*_PI_); }
	Vector2& Rotate(value_t rad)
	{
		value_t old_x = x;
		x = x * cos(rad) + y * sin(rad);
		y = y * cos(rad) - old_x * sin(rad);
		return *this;
	}
	Vector2& RotateNormalized(value_t rad)
	{
		value_t old_x = x;
		rad *= 2 * _PI_;
		x = x * cos(rad) + y * sin(rad);
		y = y * cos(rad) - old_x * sin(rad);
		return *this;
	}
	//Vector2 & Normal();
	//Vector2 & Normalize();

	Vector2 & operator = (const Vector2 & v) { x = v.x; y = v.y; return *this; }
	Vector2 & operator = (const value_t & f) { x = f; y = f; return *this; }
	Vector2 operator - (void) { return Vector2(-x, -y); }

	bool operator == (const Vector2 & v) const { return (x == v.x) && (y == v.y); }
	bool operator != (const Vector2 & v) const { return (x != v.x) || (y != v.y); }

	Vector2 operator + (const Vector2 & v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator - (const Vector2 & v) const { return Vector2(x - v.x, y - v.y); }

	Vector2 & operator += (const Vector2 & v) { x += v.x; y += v.y; return *this; }
	Vector2 & operator -= (const Vector2 & v) { x -= v.x; y -= v.y; return *this; }
	
	Vector2 operator + (value_t v) const { return Vector2(x + v, y + v); }
	Vector2 operator - (value_t v) const { return Vector2(x - v, y - v); }
	Vector2 operator * (value_t v) const { return Vector2(x * v, y * v); }
	Vector2 operator / (value_t v) const { return Vector2(x / v, y / v); }

	Vector2 & operator += (value_t v) { x += v; y += v; return *this; }
	Vector2 & operator -= (value_t v) { x -= v; y -= v; return *this; }
	Vector2 & operator *= (value_t v) { x *= v; y *= v; return *this; }
	Vector2 & operator /= (value_t v) { x /= v; y /= v; return *this; }

	value_t operator * (const Vector2 & v) const { return x * v.x + y * v.y; }

};
#undef _PI_
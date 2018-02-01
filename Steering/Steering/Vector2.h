#ifndef __VECTOR2_H_
#define __VECTOR2_H_

#include "math.h"

class Vector2
{
public:
	double x;
	double y;

	Vector2() : x(0.0f), y(0.0f) {};
	Vector2(double _x, double _y) : x(_x), y(_y) {};

	void Normalize(){
		double length = sqrt(x*x + y*y);
		if(length != 0)
		{
			x /= length;
			y /= length;
		}
	}
	Vector2 Normalized(){
		double length = sqrt(x*x + y*y);
		if (length != 0)
		{
			return Vector2(x / length, y /= length);
		}
		else
			return *this;
	}
	double LengthSquare(){ return x*x + y*y; }
	double Length() const { return sqrt(x*x + y*y); }

	Vector2 operator*(double const&n){ return Vector2( x*n, y*n ); }
	Vector2 operator/(double const&n){ return Vector2( x/n, y/n ); }
	Vector2 operator+(Vector2 const&_v){ return Vector2( x + _v.x, y + _v.y); }
	Vector2 operator-(Vector2 const&_v){ return Vector2( x - _v.x, y - _v.y); }

	Vector2& operator*=(double const&n){
		x *=n;
		y *=n;
		return *this;
	}
	Vector2& operator/=(double const&n){
		x /=n;
		y /=n;
		return *this;
	}
	Vector2& operator+=(Vector2 const&_v){
		x += _v.x;
		y += _v.y;
		return *this;
	}
	Vector2& operator-=(Vector2 const&_v){ 
		x -= _v.x;
		y -= _v.y;
		return *this;
	}
	float angle(Vector2 const&_v){
		double l = Length() * _v.Length();
		if(l != 0)
		{
			return (float) acos((x*_v.x + y*_v.y)/l);
		}
		return 0.0f;
	}
	bool isLeft(Vector2 const&_v){
		return (bool)((x*_v.y - y*_v.x) > 0.0f);
	}
	Vector2 Transform(Vector2 const&_v){
		return Vector2(x*_v.x - y*_v.y, x*_v.y + y*_v.x);
	}

};

const Vector2 ORIGIN2(0.0f, 0.0f);



#endif
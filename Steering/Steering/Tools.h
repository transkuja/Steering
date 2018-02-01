#ifndef __TOOLS_H_
#define __TOOLS_H_

#include "Vector2.h"

#define EPSILON 0.00001f
#define PI 3.14159265f

class Tools
{
public:

	static double lerp(double a, double b, double f) { return a * (1-f) + b * f; }

	static double degreetoradian(double a) { return a * 0.01745329251945f; }

	static double radiantodegreee(double a) { return a * 57.29577951471995f; }

	static double min(double a, double b){ return a < b ? a : b; }

	static double max(double a, double b){ return a > b ? a : b; }

	static Vector2 truncate(Vector2 _vector, double lengthLimit){
		double length = _vector.Length();
		if( length > lengthLimit){
			_vector.Normalize() ;
			return _vector * lengthLimit;
		}
		else return _vector;
	}

	static bool zeroByEpsilon(double _value){ return _value > -EPSILON && _value < EPSILON; }

	static int global_counter;
	static void updateCount(int _countMax)
	{
		global_counter++;
		if (global_counter == _countMax)
		{
			global_counter = 0;
		}
	}
};


#endif
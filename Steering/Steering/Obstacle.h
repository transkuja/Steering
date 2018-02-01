#ifndef __OBSTACLE_H_
#define __OBSTACLE_H_

#include "Vector2.h"

class Obstacle
{
public:
	Vector2 m_position;
	double m_radius;

	Obstacle(Vector2 position, double radius){ 
		m_position = position;
		m_radius = radius;
	}
};


#endif

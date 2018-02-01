#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <vector>

#include "Behavior.h"
#include "Tools.h"
#include "Vector2.h"

typedef std::pair<float, Behavior*> typeWBehavior;

class Entity
{
protected:
	double m_mass;
	Vector2 m_position;
	Vector2 m_velocity;
	double m_maxForce;
	double m_maxSpeed;
	
	//last force to render
	Vector2 m_lastForce;
	Vector2 m_lastR;

	int m_index;

	std::vector<typeWBehavior*> m_behaviors; // All the behaviors and their weight associated

public:
	Obstacle* pTarget;

	Entity();
	Entity(double _mass, Vector2 _position, Vector2 _velocity, double _maxForce, double _maxSpeed);
	~Entity();

	Vector2 Steer(double _dT);
	void Update(double _dT);

	void AddBehavior(Behavior* _behavior, float _weight);	
	void ClearBehaviors();

	double getMass(){ return m_mass; }
	Vector2 getPosition(){ return m_position; }
	Vector2 getVelocity(){ return m_velocity; }
	double getMaxForce(){ return m_maxForce; }
	double getMaxSpeed(){ return m_maxSpeed; }
	Vector2 getLastForce(){ return m_lastForce; }
	Vector2 getLastR(){ return m_lastR; }
	int getIndex(){ return m_index; }

	void setMass(double _mass){ m_mass = _mass; }
	void setPosition(Vector2 _position){ m_position = _position; }
	void setVelocity(Vector2 _velocity){ m_velocity = _velocity; }
	void setMaxForce(double _maxForce){ m_maxForce = _maxForce; }
	void setMaxSpeed(double _maxSpeed){ m_maxSpeed = _maxSpeed; }
	void setLastR(Vector2 _lastR){ m_lastR = _lastR; }
	void setIndex(int _index){ m_index = _index; }

};





#endif
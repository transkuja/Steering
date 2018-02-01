#include "Entity.h"

Entity::Entity() :  
	m_mass(1.0f), 
	m_position(Vector2(0.0f, 0.0f)), 
	m_velocity(Vector2(0.0f, 0.0f)), 
	m_maxForce(100.0f), 
	m_maxSpeed(100.0f), 
	m_lastForce(Vector2(0.0f, 0.0f)),
	m_lastR(Vector2(1.0f, 0.0f)),
	pTarget(NULL)
{};

Entity::Entity(double _mass, Vector2 _position, Vector2 _velocity, double _maxForce, double _maxSpeed) :
	m_mass(_mass), 
	m_position(_position), 
	m_velocity(_velocity), 
	m_maxForce(_maxForce), 
	m_maxSpeed(_maxSpeed), 
	m_lastForce(Vector2(0.0f, 0.0f)) ,
	m_lastR(Vector2(1.0f, 0.0f)),
	pTarget(NULL)
{};

Entity::~Entity()
{
	ClearBehaviors();
};

void Entity::AddBehavior(Behavior* _behavior, float _weight)	
{ 
	m_behaviors.push_back(new typeWBehavior(_weight, _behavior)); 
};	

void Entity::ClearBehaviors()									
{ 
	for(int i = 0; i < (int)m_behaviors.size(); i++)
	{
		typeWBehavior* p = m_behaviors.back();
		delete(p->second);
		delete(p);
		m_behaviors.pop_back();
	}
	m_behaviors.clear(); 
};

Vector2 Entity::Steer(double _dt)
{
	Vector2 steeringDirection;
	for(int i = 0; i < (int)m_behaviors.size(); i++)
	{
		typeWBehavior* t = m_behaviors[i];
		Behavior* b = (Behavior*) t->second;
		steeringDirection += (t->second->Update(_dt) * t->first);
	}
	return steeringDirection;
}

void Entity::Update( double _dt )
{
	Vector2 steeringDirection = Steer(_dt);

	if(! Tools::zeroByEpsilon(steeringDirection.LengthSquare()))
	{
		Vector2 steeringForce = Tools::truncate(steeringDirection, m_maxForce);
		Vector2 acceleration = steeringForce / m_mass;
		m_velocity = Tools::truncate(m_velocity + acceleration * _dt, m_maxSpeed);

		m_lastForce = steeringForce;
	}
	else
	{
		m_lastForce = Vector2(0.0f,0.0f);
	}
	m_position += m_velocity * _dt;

}

#include "stdafx.h"
#include "AI/Behaviour.h"

namespace crea
{
	Behaviour::Behaviour(Entity* _entity, short _poids)
		: m_entity(_entity), m_poids(_poids), m_desiredVelocity(Vector2f(0, 0)), m_steering(Vector2f(0, 0)), m_offset(Vector2f(0, 0))
	{
	};

	Vector2f& Seek::Update(double _dT)
	{
		m_desiredVelocity = m_target->getPosition() - m_entity->getPosition();
		m_desiredVelocity.normalize();
		m_desiredVelocity *= m_entity->getComponent<Agent>()->getMaxSpeed();
		m_steering = m_desiredVelocity - m_entity->getVelocity();
		return m_steering * m_poids;
	}

	Behaviour::~Behaviour()
	{
		delete m_entity;
	}
}
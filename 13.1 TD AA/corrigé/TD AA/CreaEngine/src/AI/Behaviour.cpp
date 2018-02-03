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

	Vector2f& Flee::Update(double _dT)
	{
		m_desiredVelocity = m_entity->getPosition() - m_target->getPosition();
		m_desiredVelocity.normalize();
		m_desiredVelocity *= m_entity->getComponent<Agent>()->getMaxSpeed();
		m_steering = m_desiredVelocity - m_entity->getVelocity();
		return m_steering * m_poids;
	}

	Vector2f& Pursuit::Update(double _dT)
	{
		Vector2f targetOffset = m_entity->getPosition() - m_target->getPosition();
		double fT = MathTools::min(m_fTmax, targetOffset.length());
		Vector2f prediction = m_target->getPosition() + m_target->getVelocity() * fT;
		m_desiredVelocity = prediction - m_entity->getPosition();
		m_desiredVelocity.normalize();
		m_desiredVelocity *= m_entity->getComponent<Agent>()->getMaxSpeed();
		m_steering = m_desiredVelocity - m_entity->getVelocity();
		return m_steering * m_poids;
	}

	Vector2f& Evasion::Update(double _dT)
	{
		Vector2f targetOffset = m_entity->getPosition() - m_target->getPosition();
		double fT = MathTools::min(m_fTmax, targetOffset.length());
		Vector2f prediction = m_target->getPosition() + m_target->getVelocity() * fT;
		m_desiredVelocity = m_entity->getPosition() - prediction;
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
/***************************************************/
/* Nom:	Transformable.h
/* Description: Transformable
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef _Transformable_H
#define _Transformable_H

#include "Core\Math.h"
#include "Core\TimeManager.h"

namespace crea
{
	class FloatRect;
	class Vector2f;

	class CREAENGINE_API Transform
	{
		Vector2f m_vPosition;
	public:

		Transform()
		{
		}

		~Transform()
		{
		}

		void move(Vector2f& _v) { m_vPosition += _v; }
		void setPosition(Vector2f& _v) { m_vPosition = _v; }
		Vector2f& getPosition() { return m_vPosition; }
		virtual FloatRect transformRect(const FloatRect&) { return FloatRect(); };
	};

	class CREAENGINE_API Transformable
	{
		Transform m_Transform;
		Vector2f m_vVelocity;

	public:

		Transformable()
		{
		}

		~Transformable()
		{
		}

		Transform& getTransform() { return m_Transform; }
		void move(Vector2f& _v) { m_Transform.move(_v); }
		void setPosition(Vector2f& _v) 
		{
			float frameTime = (float)TimeManager::getSingleton()->getFrameTime().asSeconds();
			if (frameTime)
			{
				m_vVelocity = Vector2f(_v - getPosition());
				m_vVelocity /= frameTime;
			}
			m_Transform.setPosition(_v); 
		}
		Vector2f& getPosition() { return m_Transform.getPosition(); }
		Vector2f& getVelocity() { return m_vVelocity; }

		void addForce(Vector2f _force, float _maxSpeed)
		{
			m_vVelocity += _force;
			if (m_vVelocity.length() > _maxSpeed)
			{
				m_vVelocity.normalize();
				m_vVelocity *= _maxSpeed;
			}
		}
	};

} // namespace crea

#endif // _Transformable_H
/***************************************************/
/* Nom:	Agent.h
/* Description: Agent
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Agent_H_
#define __Agent_H_

#include "Core\Component.h"
#include "AI.h"

namespace crea
{
	class ISprite;
	class StateMachine;

	class CREAENGINE_API Agent : public Component
	{
		short m_nStrength;
		short m_nDexterity;
		short m_nIntelligence;
		short m_nHealth;
		short m_nSize;	// Unit size (ex: 2 is a 2*2 nodes unit)
		short m_nCapability; // Unit capability (ex: 0 is a Ground unit)
		float m_maxSpeed;
		float m_maxForce;
		float m_mass;


	public:

		Agent();
		virtual ~Agent();
		std::vector<Behaviour*> m_behaviours;

		inline void setStrength(short _iStength) { m_nStrength = _iStength; }
		inline void setDexterity(short _iDexterity) { m_nDexterity = _iDexterity; }
		inline void setIntelligence(short _iIntelligence) { m_nIntelligence = _iIntelligence; }
		inline void setHealth(short _iHealth) { m_nHealth = _iHealth; }
		inline void setSize(short _iSize) { m_nSize = _iSize; }
		inline void setCapability(short _iCapability) { m_nCapability = _iCapability; }

		inline void setMaxSpeed(float _maxSpeed) { m_maxSpeed = _maxSpeed; }
		inline void setMaxForce(float _maxForce) { m_maxForce = _maxForce; }
		inline void setMass(float _mass) { m_mass = _mass; }

		inline short getStrength() const { return m_nStrength; }
		inline short getDexterity() const { return m_nDexterity; }
		inline short getIntelligence() const { return m_nIntelligence; }
		inline short getHealth() const { return m_nHealth; }
		inline short getSize() const { return m_nSize; }
		inline short getCapability() const { return m_nCapability; }

		inline short getMaxSpeed() const { return m_maxSpeed; }
		inline short getMaxForce() const { return m_maxForce; }
		inline short getMass() const { return m_mass; }

		bool loadFromFileJSON(string _filename);

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

	private:

	};

} // namespace crea

#endif

#include "stdafx.h"

#include "AI\Agent.h"
#include "Core\GameManager.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	Agent::Agent() : m_nStrength(10), m_nDexterity(10), m_nIntelligence(10), m_nHealth(10), m_nSize(1), m_nCapability(0), m_maxSpeed(20.0f), m_mass(1.0f), m_maxForce(50.0f)
	{
	}

	Agent::~Agent()
	{
	}

	bool Agent::loadFromFileJSON(string _filename)
	{
		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		m_nStrength = (short) root["Strength"].asInt();
		m_nDexterity = (short) root["Dexterity"].asInt();
		m_nIntelligence = (short) root["Intelligence"].asInt();
		m_nHealth = (short) root["Health"].asInt();
		m_nSize = (short)root["Size"].asInt();
		m_nCapability = (short)root["Capability"].asInt();

		m_maxSpeed = root["MaxSpeed"].asFloat();
		m_maxForce = root["MaxForce"].asFloat();
		m_mass = root["Mass"].asFloat();

		return true;
	}

	bool Agent::init()
	{
		return true;
	}

	bool Agent::update()
	{
		Vector2f motion = Vector2f(0, 0);
		for (auto behaviour = m_behaviours.begin(); behaviour != m_behaviours.end(); ++behaviour)
		{
			Behaviour* _behaviour = (Behaviour*)*behaviour;
			motion += (*behaviour)->Update(TimeManager::getSingleton()->getFrameTime().asSeconds());
		}

		getEntity()->addForce(motion * TimeManager::getSingleton()->getFrameTime().asSeconds(), m_maxSpeed);
		return true;
	}

	bool Agent::draw()
	{
		return true;
	}

	bool Agent::quit()
	{
		return true;
	}

} // namespace crea

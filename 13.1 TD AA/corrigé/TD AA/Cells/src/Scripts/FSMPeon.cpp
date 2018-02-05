#include "stdafx.h"

#include "Scripts\FSMPeon.h"
#include "Scripts\Messages.h"
#include <string.h>


//Add new states here
enum States {
	STATE_Spawn,
	STATE_Live,
	STATE_Die,
	STATE_Seek,
	STATE_Flee,
	STATE_Pursuit,
	STATE_Evasion,
	STATE_Arrival,
	STATE_ObstacleAvoidance,
	STATE_Wander,
	STATE_PathFollowing,

	STATE_UnalignedCollisionAvoidance,
	STATE_Separation,
	STATE_Cohesion,
	STATE_Alignment,

	STATE_Flocking,
	STATE_LeadFollowing,
	STATE_FormationV,
	STATE_FormationLine,

	STATE_FormationCircle,
	STATE_Formation2Lvl,
	STATE_FormationOfFormations,
	STATE_FormationDynamic
};

FSMPeon::FSMPeon()
{

}

FSMPeon::~FSMPeon()
{

}

bool FSMPeon::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine

		OnMsg(MSG_Seek)
			SetState(STATE_Seek);
		OnMsg(MSG_Flee)
			SetState(STATE_Flee);
		OnMsg(MSG_Pursuit)
			SetState(STATE_Pursuit);
		OnMsg(MSG_Evasion)
			SetState(STATE_Evasion);
		OnMsg(MSG_Arrival)
			SetState(STATE_Arrival);
		OnMsg(MSG_ObstacleAvoidance)
			SetState(STATE_ObstacleAvoidance);
		OnMsg(MSG_Wander)
			SetState(STATE_Wander);

		OnMsg(MSG_Die)
			SetState(STATE_Die);		
		
		OnMsg(MSG_Hit)
			m_iLife -= 20;
			if (m_iLife <= 0)
			{
				SetState(STATE_Die);
			}
			//if (m_pFSMPeonLive)
			//{
			//	m_pFSMPeonLive->States(_event, _msg, _state); // CB: propagate msg to sub-state
			//}

		OnMsg(MSG_Boost)
			m_pAgent->setDexterity(m_pAgent->getDexterity()+1);

		OnOtherMsg()
		//	if (m_pFSMPeonLive)
		//	{
		//		m_pFSMPeonLive->States(_event, _msg, _state); // CB: propagate msg to sub-state
		//	}

		///////////////////////////////////////////////////////////////
		State(STATE_Spawn)
		OnEnter
			// Get Entity
			m_pEntity = getEntity();
			// Get CharacterController
			m_pCharacterController = m_pEntity->getComponent<CharacterController>();
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();

			m_iLife = 100;

		OnUpdate
			/*SetState(STATE_Live);*/

		///////////////////////////////////////////////////////////////
		//State(STATE_Live)
		//OnEnter
		//	m_pFSMPeonLive = new FSMPeonLive();
		//	m_pFSMPeonLive->Initialize(getEntity());

		//OnUpdate
		//	m_pFSMPeonLive->Update();
		//	if (m_iLife <= 0)
		//	{
		//		SetState(STATE_Die);
		//	}

		//OnExit
		//	delete m_pFSMPeonLive;
		//	m_pFSMPeonLive = nullptr;
			
		///////////////////////////////////////////////////////////////
		State(STATE_Die)
		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Die);

		////////////////////////////////////////////////////////////
		State(STATE_Seek)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new Seek(m_pEntity, GameManager::getSingleton()->getEntity("mouse")));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);

			m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_Flee)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new Flee(m_pEntity, GameManager::getSingleton()->getEntity("mouse")));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);

			m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_Pursuit)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new Pursuit(m_pEntity, GameManager::getSingleton()->getEntity("mouse"), 2.0f));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);

			m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_Evasion)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new Evasion(m_pEntity, GameManager::getSingleton()->getEntity("mouse"), 2.0f));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);

			m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_Arrival)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new Arrival(m_pEntity, GameManager::getSingleton()->getEntity("mouse"), 350.0f));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);

			Vector2f targetOffset = GameManager::getSingleton()->getEntity("mouse")->getPosition() - m_pEntity->getPosition();
			double distance = targetOffset.length();
			if (distance < 10.0f)
			{
				m_pAgent->m_behaviours.clear();
				m_pCharacterController->move(Vector2f(0, 0));
				m_pCharacterController->setAction(kAct_Default);
				SetState(STATE_Spawn);
			}
			else
				m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_ObstacleAvoidance)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new ObstacleAvoidance(m_pEntity, 32.0f, 100.0f, PhysicsManager::getSingleton()->getStaticCollidersAsVector(), 3));
			m_pAgent->m_behaviours.push_back(new Seek(m_pEntity, GameManager::getSingleton()->getEntity("mouse"), 2));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);
			if (PhysicsManager::getSingleton()->isColliding(m_pEntity->getComponent<Collider>()))
			{
				m_pAgent->m_behaviours.at(0)->UpdatePoids(5);
				m_pAgent->m_behaviours.at(1)->UpdatePoids(0);
			}
			else
			{
				m_pAgent->m_behaviours.at(0)->UpdatePoids(3);
				m_pAgent->m_behaviours.at(1)->UpdatePoids(2);
			}
			m_pCharacterController->move(m_pEntity->getVelocity());

		State(STATE_Wander)
		OnEnter
			m_pAgent->m_behaviours.clear();
			m_pAgent->m_behaviours.push_back(new ObstacleAvoidance(m_pEntity, 32.0f, 100.0f, PhysicsManager::getSingleton()->getStaticCollidersAsVector(), 3));
			m_pAgent->m_behaviours.push_back(new Wander(m_pEntity, 32.0f, 32.0f, 2));

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Walk);
			if (PhysicsManager::getSingleton()->isColliding(m_pEntity->getComponent<Collider>()))
			{
				m_pAgent->m_behaviours.at(0)->UpdatePoids(5);
				m_pAgent->m_behaviours.at(1)->UpdatePoids(0);
			}
			else
			{
				m_pAgent->m_behaviours.at(0)->UpdatePoids(3);
				m_pAgent->m_behaviours.at(1)->UpdatePoids(2);
			}
			m_pCharacterController->move(m_pEntity->getVelocity());
EndStateMachine
}
/***************************************************/
/* Nom:	PhysicsManager.h
/* Description: PhysicsManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __PhysicsManager_H_
#define __PhysicsManager_H_

namespace crea
{
	class Collider;


	class CREAENGINE_API Collision2D
	{
	public:
		Collider* m_pCollider;
		Collider* m_pOtherCollider;
		bool m_bUpdated;
	};

	class CREAENGINE_API MapStringCollider : public map<string, Collider*> {};
	class CREAENGINE_API MapColliderCollision2D : public multimap<Collider*, Collision2D*> {};

	class CREAENGINE_API PhysicsManager
	{
		MapStringCollider m_StaticColliders;
		vector<Collider*>* m_StaticCollidersAsVector = nullptr;

		MapStringCollider m_DynamicColliders;
		MapColliderCollision2D m_Collisions2D;

		Map* m_pCurrentMap; // Physics materials

		PhysicsManager();

	public:
		virtual ~PhysicsManager();

		static PhysicsManager* getSingleton();

		void addStaticCollider(string& _szName, Collider* _pCollider);

		Collider* getDynamicCollider(string& _szName, bool _bCloned = false);

		Collider* getStaticCollider(string& _szName, bool _bCloned = false);

		bool isColliding(Collider* _pCollider, bool _bWithStatic = true, bool _bWithDynamic = false, bool _bWithTrigger = false);

		void setCurrentMap(Map* _pMap);

		Map* getCurrentMap();

		MapStringCollider* getStaticColliders() { return &m_StaticColliders; }
		vector<Collider*>* getStaticCollidersAsVector() { 
			if (m_StaticCollidersAsVector == nullptr)
			{
				m_StaticCollidersAsVector = new vector<Collider*>();

				for (auto it = m_StaticColliders.begin(); it != m_StaticColliders.end(); ++it)
					m_StaticCollidersAsVector->push_back(it->second);
			}

			return m_StaticCollidersAsVector;
		}

		MapStringCollider* getDynamicColliders() { return &m_DynamicColliders; }

		bool onEnterCollision(Collider* _pCollider, Collider* _pOtherCollider);
			
		bool updateCollision(Collider* _pCollider, Collider* _pOtherCollider);

		bool cleanupCollisions();

		bool init();

		bool update();
			
		bool draw();

		void clear();
	};

} // namespace crea

#endif

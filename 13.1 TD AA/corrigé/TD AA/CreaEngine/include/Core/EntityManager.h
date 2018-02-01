/***************************************************/
/* Nom:	EntityManager.h
/* Description: EntityManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __EntityManager_H_
#define __EntityManager_H_

#include <map>
#include <list>
using namespace std;

namespace crea
{
	// Prédéfinitions
	class Entity;
	class TextRenderer;
	class SpriteRenderer;
	class MapRenderer;
	class Animator;
	class Vector2f;
	class Script;
	class ScriptFactory;

	class CREAENGINE_API MapStringTextRenderer : public map<string, TextRenderer*> {};
	class CREAENGINE_API MapStringSpriteRenderer : public map<string, SpriteRenderer*> {};
	class CREAENGINE_API MapStringMapRenderer : public map<string, MapRenderer*> {};
	class CREAENGINE_API MapStringAnimator : public map<string, Animator*> {};
	class CREAENGINE_API MapStringScript : public map<string, Script*> {};
	class CREAENGINE_API ListEntity : public list<Entity*> {};
	class CREAENGINE_API MapObjectIDEntity : public map<objectID, Entity*> {};

	class CREAENGINE_API EntityManager
	{
		Entity* m_pRoot;

		MapStringTextRenderer m_pTextRenderers;

		MapStringSpriteRenderer m_pSpriteRenderers;

		MapStringMapRenderer m_pMapRenderers;

		MapStringAnimator m_pAnimators;

		MapStringScript m_pScripts;

		ScriptFactory* m_pScriptFactory;

		ListEntity m_pSelectedEntities;

		EntityManager();

		objectID nextFreeID;

		MapObjectIDEntity m_Entities;

	public:
		virtual ~EntityManager();

		static EntityManager* getSingleton();

		Entity* getEntity(string& _szName);

		void addEntity(Entity* _pEntity, Entity* _pParent = nullptr);

		TextRenderer* getTextRenderer(string _szName, bool _bCloned = false);

		SpriteRenderer* getSpriteRenderer(string _szName, bool _bCloned = false);

		MapRenderer* getMapRenderer(string _szName, bool _bCloned = false);

		Animator* getAnimator(string _szName, bool _bCloned = false);

		Script* getScript(string _szName, bool _bCloned = false);

		inline void setScriptFactory(ScriptFactory* _pScriptFactory) { m_pScriptFactory = _pScriptFactory; }

		void selectEntities(Vector2f _vStart, Vector2f _vEnd);

		void addSelectedEntity(Entity* _pEntity);

		void unselectEntities();

		inline ListEntity* getSelectedEntities() { return &m_pSelectedEntities; }

		void Store(Entity& _agent);

		void Remove(objectID _id);

		Entity* Find(objectID _id);

		objectID getNewObjectID();


		bool init();

		bool update();

		bool draw();

		void clear();
	};

} // namespace crea

#endif

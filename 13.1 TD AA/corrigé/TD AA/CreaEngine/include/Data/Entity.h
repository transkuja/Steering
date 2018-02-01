/***************************************************/
/* Nom:	Entity.h
/* Description: Entity
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Entity_H_
#define __Entity_H_

#include "Core\Transformable.h"
#include <list>

namespace crea
{
	class Component;

	class CREAENGINE_API Entity : public Transformable // Every entity is transformable
	{
		// Name
		string m_szName;
		objectID m_ID;

		// Composite
		Entity* m_pParent;
		list<Entity*> m_pChildren;

		// Composition
		list<Component*> m_pComponents;

		// Selection
		bool m_bSelected;

	public:
		Entity();
		virtual ~Entity();

		inline bool hasName(string& _szName) { return (m_szName == _szName); }

		inline void setName(string& _szName) { m_szName = _szName; }

		inline string& getName() { return m_szName; }

		void SetID(objectID _id) { m_ID = _id; }

		objectID GetID() { return m_ID; }

		inline void setParent(Entity* _pEntity) { m_pParent = _pEntity; }

		Entity* getEntity(string& _szName);

		Entity* getEntity(Entity* _pEntity);

		bool removeEntity(Entity* _pEntity);
		
		void addChild(Entity* _pEntity);

		void removeChild(Entity* _pEntity);

		void addComponent(Component* _pComponent);

		void removeComponent(Component* _pComponent);

		template<class T> T* getComponent();

		template<class T> void getComponents(list<T*>& _v);
			
		void selectEntities(FloatRect& _rect);

		void unselectEntities();

		bool getSelected() { return m_bSelected; }

		bool loadFromFileJSON(string& _filename);

		bool init();

		bool update();

		bool draw();

		void clear();

	};

	template<class T> T* Entity::getComponent()
	{
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			Component* pComponent = *it;
			T* ptr = dynamic_cast<T*>(pComponent);
			if (ptr != nullptr)
			{
				return ptr;
			}
			else
			{
				// Children
				for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
				{
					(*it)->getComponent<T>();
				}
			}
		}
		return nullptr;
	}

	template<class T> void Entity::getComponents(list<T*>& _l)
	{
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			Component* pComponent = *it;
			T* ptr = dynamic_cast<T*>(pComponent);
			if (ptr != nullptr)
			{
				_l.push_back(ptr);
			}
			else
			{
				// Children
				for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
				{
					(*it)->getComponents<T>(_l);
				}
			}
		}
		return;
	}
} // namespace crea

#endif

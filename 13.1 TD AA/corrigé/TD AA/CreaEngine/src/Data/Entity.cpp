#include "stdafx.h"

#include "json\json.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Data\Entity.h"
#include "Core\Component.h"
#include "AI\Agent.h"
#include "AI\ActionTable.h"
#include "Core\Script.h"
#include "Physics\Collider.h"

namespace crea
{
	Entity::Entity()
	{
		m_pParent = nullptr;
		m_szName = "Entity"; 
		m_ID = 0;
	}

	Entity::~Entity()
	{
		clear();
	}

	void Entity::addChild(Entity* _pEntity)
	{
		m_pChildren.push_back(_pEntity);
		_pEntity->setParent(this);
	}

	void Entity::removeChild(Entity* _pEntity)
	{
		_pEntity->setParent(nullptr);
		m_pChildren.remove(_pEntity);
	}

	void Entity::addComponent(Component* _pComponent)
	{
		_pComponent->setEntity(this);
		m_pComponents.push_back(_pComponent);
	}

	void Entity::removeComponent(Component* _pComponent)
	{
		_pComponent->setEntity(nullptr);
		auto a = std::remove_if(m_pComponents.begin(), m_pComponents.end(),
			[=](Component* p) { return p == _pComponent; });

		m_pComponents.erase(a);
	}

	bool Entity::init()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->init();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->init();
		}

		return true;
	}

	bool Entity::update()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->update();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->update();
		}

		return true;
	}

	bool Entity::draw()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->draw();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->draw();
		}


		return true;
	}

	void Entity::clear()
	{
		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			delete (*it);
		}
		m_pChildren.clear();

		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			//delete (*it);// CB: the components are destroyed by managers...
		}
		m_pComponents.clear();
	}

	Entity* Entity::getEntity(string& _szName)
	{
		if (hasName(_szName))
		{
			return this;
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				Entity* pEntity = (*it)->getEntity(_szName);
				if (pEntity)
				{
					return pEntity;
				}
			}
		}
		return nullptr;
	}

	Entity* Entity::getEntity(Entity* _pEntity)
	{
		if (_pEntity == this)
		{
			return this;
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				Entity* pEntity = (*it)->getEntity(_pEntity);
				if (pEntity)
				{
					return pEntity;
				}
			}
		}
		return nullptr;
	}

	bool Entity::removeEntity(Entity* _pEntity)
	{
		if (_pEntity->m_pParent != nullptr)
		{
			_pEntity->m_pParent->removeChild(_pEntity);
			return true;
		}
		return false;
	}

	void Entity::selectEntities(FloatRect& _rect)
	{
		if (_rect.contains(getPosition()))
		{
			m_bSelected = true;
			EntityManager::getSingleton()->addSelectedEntity(this);
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				(*it)->selectEntities(_rect);
			}
		}
	}

	void Entity::unselectEntities()
	{
		m_bSelected = false;

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->unselectEntities();
		}
	}

	bool Entity::loadFromFileJSON(string& _filename)
	{
		Json::Value root;
		std::ifstream entityStream(_filename, std::ifstream::binary);
		if (entityStream.fail())
		{
			cerr << "Can't open Entity file: " << _filename << endl;
			return false;
		}

		// Parse file
		entityStream >> root;

		crea::GameManager*	pGM = crea::GameManager::getSingleton();

		// Components
		Json::Value components = root["components"];
		for (unsigned int iComponent = 0; iComponent < components.size(); ++iComponent)
		{
			Json::Value component = components[iComponent];

			string szType = component["type"].asString();
			if (szType == "SpriteRenderer")
			{
				string szName = component["name"].asString();
				SpriteRenderer* pSpriteRenderer = pGM->getSpriteRenderer(szName);

				spriteName = component["sprite"].asString();
				Sprite* pSprite = pGM->getSprite(spriteName);

				textureName = component["image"].asString();
				Texture* pTexture = pGM->getTexture(textureName);
				pSprite->setTexture(pTexture);

				pSpriteRenderer->setSprite(pSprite);
				addComponent(pSpriteRenderer);
			}
			else if (szType == "Agent")
			{
				string szName = component["name"].asString();
				Agent* pAgent = pGM->getAgent(szName);

				addComponent(pAgent);
			}
			else if (szType == "Animator")
			{
				string szName = component["name"].asString();
				Animator* pAnimator = pGM->getAnimator(szName);

				string szSprite = component["sprite"].asString();
				Sprite* pSprite = pGM->getSprite(szSprite);

				pAnimator->setSprite(pSprite);
				addComponent(pAnimator);
			}
			else if (szType == "ActionTable")
			{
				string szName = component["name"].asString();
				ActionTable* pActionTable = pGM->getActionTable(szName);
				addComponent(pActionTable);
			}
			else if (szType == "Script")
			{
				string szName = component["name"].asString();
				Script* pScript = pGM->getScript(szName);
				addComponent(pScript);
			}
			else if (szType == "Collider")
			{
				string szName = component["name"].asString();
				Collider* pCollider = pGM->getDynamicCollider(szName);
				addComponent(pCollider);
			}
		}
		return true;
	}

	Entity* Entity::cloneEntity()
	{
		crea::GameManager*	pGM = crea::GameManager::getSingleton();

		if (getComponent<Agent>() != nullptr)
		{
			Agent* pAgent = pGM->getAgent(m_szName);
			addComponent(pAgent);
		}
		if (getComponent<Collider>() != nullptr)
		{
			Collider* pCollider = pGM->getDynamicCollider(m_szName);
			addComponent(pCollider);
		}
		if (getComponent<SpriteRenderer>() != nullptr)
		{
			SpriteRenderer* pSpriteRenderer = pGM->getSpriteRenderer(m_szName);

			Sprite* pSprite = pGM->getSprite(spriteName);

			Texture* pTexture = pGM->getTexture(textureName);
			pSprite->setTexture(pTexture);

			pSpriteRenderer->setSprite(pSprite);
			addComponent(pSpriteRenderer);
		}
			
		if (getComponent<Animator>() != nullptr)
		{
			Animator* pAnimator = pGM->getAnimator(m_szName);

			Sprite* pSprite = pGM->getSprite(spriteName);

			pAnimator->setSprite(pSprite);
			addComponent(pAnimator);
		}
		
		if (getComponent<ActionTable>() != nullptr)
		{
			ActionTable* pActionTable = pGM->getActionTable(m_szName);
			addComponent(pActionTable);
		}

		list<Script*> scripts;
		getComponents<Script>(scripts);
		for (auto it = scripts.begin(); it != scripts.end(); ++it)
		{
			Script* pScript = pGM->getScript(m_szName);
			addComponent(pScript);
		}

		else if (szType == "Script")
		{
			string szName = component["name"].asString();
			
		}
		else if (szType == "Collider")
		{

		}
	}


} // namespace crea

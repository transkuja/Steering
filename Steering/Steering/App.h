#ifndef __APP_H_
#define __APP_H_

#include <vector>

#include "Render.h"
#include "Entity.h"
#include "Vector2.h"
#include "Tools.h"
#include "Input.h"
#include "Obstacle.h"

class App
{
	Input input;
	Render render;
	std::vector<Obstacle*> obstacles;
	std::vector<Entity*> entities;
	int lastNumber;
	STATE oldState;
	Entity mouseEntity;

	void setEntities(STATE state);

public:
	App();
	~App();
	
	void Init();
	void Update( float _fDeltaTime );
	void DeInit();
};

#endif
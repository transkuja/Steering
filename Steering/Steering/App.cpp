#include "App.h"
#include <ctime>
#include <windows.h>
#include "Behavior.h"

int Tools::global_counter = 0;

App::App()
{
	lastNumber = 0;	
	oldState = SEEK_FLEE;
}

App::~App()
{}

void App::Init()
{
	srand((unsigned int)time(0));

	render.displayInit();
	render.setInput(&input);
	
	obstacles.push_back(new Obstacle(Vector2(200, 300), 40));
	obstacles.push_back(new Obstacle(Vector2(300, 400), 60));
	obstacles.push_back(new Obstacle(Vector2(150, 500), 50));
	obstacles.push_back(new Obstacle(Vector2(400, 200), 100));
	obstacles.push_back(new Obstacle(Vector2(600, 300), 90));
	obstacles.push_back(new Obstacle(Vector2(500, 500), 50));
	obstacles.push_back(new Obstacle(Vector2(750, 600), 40));
	obstacles.push_back(new Obstacle(Vector2(800, 400), 60));
	obstacles.push_back(new Obstacle(Vector2(650, 500), 50));
	obstacles.push_back(new Obstacle(Vector2(900, 200), 100));
	obstacles.push_back(new Obstacle(Vector2(1100, 300), 90));
	obstacles.push_back(new Obstacle(Vector2(1000, 500), 50));

	render.setObstaclesToRender(&obstacles);

	for(int i = 0; i < input.numberEntity; i++){
		entities.push_back( new Entity );
		entities[i]->setPosition(Vector2((double)(rand()%SIZE_SCREEN_Y), (double)(rand()%SIZE_SCREEN_X)));
	}

	render.setEntitiesToRender(&entities);
	lastNumber = input.numberEntity;

	setEntities(input.state);

}

void App::Update(float _fDeltaTime)
{
	input.update();

	// put a dummy entity on mouse position

	Vector2 oldPosition = mouseEntity.getPosition();
	Vector2 newPosition = input.getMousePosition();
	Vector2 newDirection = newPosition - oldPosition;
	mouseEntity.setPosition(newPosition);
	if (newDirection.Length() > 0.0f)
		mouseEntity.setVelocity(newDirection.Normalized());

	if (oldState != input.state)
		setEntities(input.state);

	if (lastNumber != input.numberEntity)
	{
		for (int i = lastNumber - 1; i >= 0; i--)
		{
			delete(entities.back());
			entities.pop_back();
		}

		entities.clear();
		lastNumber = input.numberEntity;
		for (int i = 0; i < lastNumber; i++)
		{
			entities.push_back(new Entity);
			entities[i]->setPosition(Vector2((double)(rand() % SIZE_SCREEN_X), (double)(rand() % SIZE_SCREEN_Y)));
			entities[i]->setVelocity(Vector2(10.0f, 10.0f));
			entities[i]->setIndex(i);
		}

		setEntities(input.state);
	}



	for (int i = 0; i < lastNumber; i++)
	{
		entities[i]->Update(_fDeltaTime);

		Vector2 pos = entities[i]->getPosition();
		if (pos.x < 0)
			pos.x = SIZE_SCREEN_X;
		else if (pos.x > SIZE_SCREEN_X)
			pos.x = 0;
		if (pos.y < 0)
			pos.y = SIZE_SCREEN_Y;
		else if (pos.y > SIZE_SCREEN_Y)
			pos.y = 0;
		entities[i]->setPosition(pos);
	}

	render.display();
}

void App::DeInit()
{}


void App::setEntities(STATE state)
{
	Entity* leader = nullptr;

	switch(input.state)
	{

	case SEEK_FLEE:
		leader = entities[lastNumber - 1];
		for (int i = 0; i < lastNumber - 1; i++)
		{
			entities[i]->ClearBehaviors();
			if(i%2 == 0)
				entities[i]->AddBehavior(new Seek(entities[i], leader), 2.f);
			else
				entities[i]->AddBehavior(new Flee(entities[i], leader), 2.f);
		}
		leader->ClearBehaviors();
		leader->AddBehavior(new Seek(leader, &mouseEntity), 2.f);
		break;
	case PURSUIT_EVASION:
		leader = entities[lastNumber-1];
		for(int i = 0; i < lastNumber - 1; i++)
		{
			entities[i]->ClearBehaviors();
			if(i%2 == 0)
				entities[i]->AddBehavior(new Pursuit(entities[i], leader, 1.0f), 2.f);
			else
				entities[i]->AddBehavior(new Evasion(entities[i], leader, 1.0f), 2.f);
		}
		leader->ClearBehaviors();
		leader->AddBehavior(new Seek(leader, &mouseEntity), 2.f);
		break;
	case ARRIVAL:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Arrival(entities[i], &mouseEntity, 100.0f), 2.f);
			entities[i]->AddBehavior(new ObstacleAvoidance(entities[i], 12, 100, &obstacles), 8.f);
		}
		break;

	case WANDER:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Wander(entities[i], 60.0f, 40.0f, 10.0f), 2.f);
		}
		break;
		
	case PATHFOLLOWING:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new PathFollowing(entities[i], 2.0f, 12, 100, &obstacles), 2.f);
		}
		break;

	case UCA:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new UnalignedCollisionAvoidance(entities[i], 60.0f, &entities), 2.f );
		}
		break;

	case SEPARATION:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Separation(entities[i], 60.0f, &entities), 2.f);
		}
		break;
		
	case COHESION:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Cohesion(entities[i], 60.0f, &entities), 2.f);
		}
		break;

	case ALIGNMENT:
		for(int i = 0; i < lastNumber; i++)
		{	
			entities[i]->ClearBehaviors();		
			entities[i]->AddBehavior(new Alignment(entities[i], 60.0f, &entities), 2.f);
		}
		break;

	case FLOCKING:
		for(int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Separation(entities[i], 60.0f, &entities), 2.f);
			entities[i]->AddBehavior(new Cohesion(entities[i], 60.0f, &entities), 2.f);
			entities[i]->AddBehavior(new Alignment(entities[i], 60.0f, &entities), 2.f);
		}
		break;
		
	case LEAD_FOLLOWING:
		leader = entities[lastNumber-1];
		for(int i = 0; i < lastNumber - 1; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Separation(entities[i], 60.0f, &entities), 2.f);
			entities[i]->AddBehavior(new LeadFollowing(entities[i], leader, 180.f, 1.57f, 80.f, 50.f), 2.f);
		}
		leader->ClearBehaviors();
		leader->AddBehavior(new Arrival(leader, &mouseEntity, 100.0f), 2.f);
		break;

	case SWARMING:
		for (int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new Swarming(entities[i], &mouseEntity, 100), 2.f);
		}
		break;	
	
	case FORMATION_L:
			for (int i = 0; i < lastNumber; i++)
			{
				entities[i]->ClearBehaviors();
				entities[i]->AddBehavior(new FormationV(entities[i], &mouseEntity, false, 10, i, lastNumber, 20.0f, 100.0f, 0.0f), 2.f);
			}
			break;
	case FORMATION_V:
		for (int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new FormationV(entities[i], &mouseEntity, true, 10, i, lastNumber, 50.0f, 100.0f, Tools::degreetoradian(45.0f)), 2.f);
		}
		break;
	case FORMATION_CIRCLE:
		for (int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new FormationCircle(entities[i], &mouseEntity, false, 10, i, lastNumber, 20.0f, 100.0f, -90.0f, 90.0f, 60.0f), 2.f);
		}
		break;
	case FORMATION_2LEVEL:
		for (int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new FormationV(entities[i], &mouseEntity, true, 10, i, lastNumber, 20.0f, 100.0f, Tools::degreetoradian(45.0f)), 2.f);
			entities[i]->AddBehavior(new ObstacleAvoidance(entities[i], 12, 100, &obstacles), 8.f);
		}
		break;
	case FORMATION_OF_FORMATIONS:

			for (int i = 0; i < min(3, lastNumber); i++)
			{
				entities[i]->ClearBehaviors();
				entities[i]->AddBehavior(new FormationV(entities[i], &mouseEntity, false, 10, i, 3, 200.0f, 100.0f, Tools::degreetoradian(-45.0f)), 2.f);
			}

			for (int i = 3; i < min(13, lastNumber); i++)
			{
				entities[i]->ClearBehaviors();
				entities[i]->AddBehavior(new FormationCircle(entities[i], entities[0], false, 10, i-3, 10, 20.0f, 100.0f, 0.0f, 360.0f, 60.0f), 2.f);
			}

			for (int i = 13; i < min(23, lastNumber); i++)
			{
				entities[i]->ClearBehaviors();
				entities[i]->AddBehavior(new FormationCircle(entities[i], entities[2], false, 10, i-13, 10, 20.0f, 100.0f, 0.0f, 360.0f, 60.0f), 2.f);
			}

			for (int i = 23; i < lastNumber; i++)
			{
				entities[i]->ClearBehaviors();
				entities[i]->AddBehavior(new FormationV(entities[i], entities[1], false, 10, i - 23, lastNumber - 23, 20.0f, 100.0f, Tools::degreetoradian(0.0f)), 2.f);
			}
			break;
	case FORMATION_DYNAMIC:
		for (int i = 0; i < lastNumber; i++)
		{
			entities[i]->ClearBehaviors();
			entities[i]->AddBehavior(new FormationDynamic(entities[i], &mouseEntity, false, 10, i, lastNumber, 20.0f, 100.0f, -90.0f, 90.0f, 60.0f), 2.f);
		}
		break;
	}
	oldState = input.state;

}
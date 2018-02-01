#ifndef __INPUT_H_
#define __INPUT_H_

#include "SDL.h"
#include "Vector2.h"

enum STATE{ SEEK_FLEE, PURSUIT_EVASION, ARRIVAL, WANDER, PATHFOLLOWING, UCA, SEPARATION, COHESION, ALIGNMENT, FLOCKING, LEAD_FOLLOWING,
	SWARMING, FORMATION_L, FORMATION_V, FORMATION_CIRCLE, FORMATION_2LEVEL, FORMATION_OF_FORMATIONS, FORMATION_DYNAMIC
};

#define RELEASED 0
#define PRESSED 1

class Input
{
	SDL_Event m_event;


	char key[SDLK_LAST];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
	char quit;

public:
	Input();
	~Input();
	
	STATE state;
	bool drawForce;
	bool drawVelocity;
	bool drawWanderCircle;

	int numberEntity;

	void update();
	void pollEvent();

	Vector2 getMousePosition(){ return Vector2(mousex, mousey); }
};

#endif
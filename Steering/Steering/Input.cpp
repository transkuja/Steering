#include "Input.h"

Input::Input()
{
	memset(this,0,sizeof(*this));
	state = FORMATION_DYNAMIC;
	drawForce = true;
	drawVelocity = true;
	drawWanderCircle = true;
	numberEntity = 10;
}

Input::~Input()
{}

void Input::pollEvent()
{
		mousebuttons[SDL_BUTTON_WHEELUP] = RELEASED;
		mousebuttons[SDL_BUTTON_WHEELDOWN] = RELEASED;
		mousebuttons[SDL_BUTTON_RIGHT] = RELEASED;
		mousebuttons[SDL_BUTTON_LEFT] = RELEASED;
		

		while ( SDL_PollEvent(&m_event) ) {
		switch (m_event.type) {
		case SDL_KEYDOWN :
			switch(m_event.key.keysym.sym) {
			case SDLK_LEFT:
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_F1:
				state = SEEK_FLEE;
				break;
			case SDLK_F2:
				state = PURSUIT_EVASION;
				break;
			case SDLK_F3:
				state = ARRIVAL;
				break;
			case SDLK_F4:
				state = WANDER;
				break;
			case SDLK_F5:
				state = PATHFOLLOWING;
				break;
			case SDLK_F6:
				state = UCA;
				break;
			case SDLK_F7:
				state = SEPARATION;
				break;
			case SDLK_F8:
				state = COHESION;
				break;
			case SDLK_F9:
				state = ALIGNMENT;
				break;
			case SDLK_F10:
				state = FLOCKING;
				break;
			case SDLK_F11:
				state = LEAD_FOLLOWING;
				break;
			case SDLK_q:
				state = SWARMING;
				break;
			case SDLK_w:
				state = FORMATION_L;
				break;
			case SDLK_e:
				state = FORMATION_V;
				break;			
			case SDLK_r:
				state = FORMATION_CIRCLE;
				break;				
			case SDLK_t:
				state = FORMATION_2LEVEL;
				break;				
			case SDLK_y:
				state = FORMATION_OF_FORMATIONS;
				break;				
			case SDLK_u:
				state = FORMATION_DYNAMIC;
				break;				
			case SDLK_f:
				drawForce = !drawForce;
				break;
			case SDLK_v:
				drawVelocity = !drawVelocity;
				break;
			case SDLK_c:
				drawWanderCircle = ! drawWanderCircle;
				break;
			case SDLK_1:
				numberEntity = 1;
				break;
			case SDLK_2:
				numberEntity = 2;
				break;
			case SDLK_3:
				numberEntity = 4;
				break;
			case SDLK_4:
				numberEntity = 10;
				break;
			case SDLK_5:
				numberEntity = 20;
				break;
			case SDLK_6:
				numberEntity = 50;
				break;
			case SDLK_7:
				numberEntity = 100;
				break;
			case SDLK_8:
				numberEntity = 250;
				break;
			case SDLK_9:
				numberEntity = 500;
				break;
			case SDLK_0:
				numberEntity = 1000;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			mousex=m_event.motion.x;
			mousey=m_event.motion.y;
			mousexrel=m_event.motion.xrel;
			mouseyrel=m_event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mousebuttons[m_event.button.button]=PRESSED;

			break;
		case SDL_MOUSEBUTTONUP:
			if (m_event.button.button!=SDL_BUTTON_WHEELUP && m_event.button.button!=SDL_BUTTON_WHEELDOWN)
				mousebuttons[m_event.button.button]=RELEASED;
			break;
		case SDL_QUIT:
			exit(0);
			break;
		default:
			break;
		}
	}
}

void Input::update()
{

	pollEvent();

	if(mousebuttons[SDL_BUTTON_LEFT] == PRESSED )
	{
	}
}
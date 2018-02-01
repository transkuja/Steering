#ifndef __RENDER_H_
#define __RENDER_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <map>
#include <string>

#define SIZE_SCREEN_X 1280
#define SIZE_SCREEN_Y 720

#define PATH_ASSET "../assets/"

#include "Entity.h"
#include "Input.h"
#include "Obstacle.h"
#include "Input.h"
#include <vector>


class Render
{

	SDL_Surface* m_pScreen;
	std::map<std::string, SDL_Surface*> m_image;

	TTF_Font *m_font;
	TTF_Font *m_fontMap;

	SDL_Color m_textColor;
	SDL_Color m_textColorBlack;

	SDL_Surface* loadImage( char* _pFilename );
	void draw(std::string _image, int _x, int _y);
	void drawText(std::string _text, int _x, int _y);

	std::vector<Entity*>* m_entitiesToRender;
	std::vector<Obstacle*>* m_obstaclesToRender;

	Input* m_input;

	void drawEntities();
	void drawEntity(Entity* ent, int R, int G, int B, int Rext, int Gext, int Bext);
	void drawObstacles();

public :
	Render();
	void loadRendererData();
	int displayInit();
	void displayExit();
	void display();
	void displayGame();
	void PutPixel(int x, int y, unsigned long pixel);
	void drawLine(int x1,int y1, int x2,int y2, int R, int G, int B);
	void drawCircle (int rayon, int x_centre, int y_centre, int R, int G, int B);

	void setInput(Input* _input){ m_input = _input; }
	void setEntitiesToRender(std::vector<Entity*>* _entities) { m_entitiesToRender = _entities; };
	void setObstaclesToRender(std::vector<Obstacle*>* _obstacles)  { m_obstaclesToRender = _obstacles; };
};




#endif

#include "Render.h"


Render::Render()
{
	m_entitiesToRender = NULL;
	m_obstaclesToRender = NULL;
	m_input = NULL;
}

SDL_Surface* Render::loadImage( char* _pFilename )
{

	SDL_Surface* loadedImage = NULL; 
	SDL_Surface* optimizedImage = NULL;

	char pathImage[255];
	sprintf_s(pathImage, "%s%s", PATH_ASSET, _pFilename);

	loadedImage = IMG_Load( pathImage );
	return loadedImage;

	return NULL;
}


void Render::loadRendererData()
{
	m_image["background"] = (SDL_Surface*)malloc(sizeof(SDL_Surface));

	char pathFont[255];
	sprintf_s(pathFont, "%sarial.ttf", PATH_ASSET);
	m_font = TTF_OpenFont(pathFont, 20);
	m_fontMap = TTF_OpenFont(pathFont, 12);

	m_textColorBlack.r = 0;
	m_textColorBlack.g = 0;
	m_textColorBlack.b = 0;

	m_textColor.r = 255;
	m_textColor.g = 0;
	m_textColor.b = 0;
}

int Render::displayInit()
{
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
		return EXIT_FAILURE;
	}

	if( TTF_Init() == -1 )
	{ 
		return EXIT_FAILURE;
	} 

	loadRendererData();

	atexit( SDL_Quit );
	m_pScreen = SDL_SetVideoMode( SIZE_SCREEN_X, SIZE_SCREEN_Y, 32, SDL_HWSURFACE );

	if( m_pScreen == NULL )
	{
		printf( "Can't set video mode: %s\n", SDL_GetError( ) );
		return EXIT_FAILURE;
	}   

	SDL_WM_SetCaption( "CREASTEER", NULL );


	return 0;
}

void Render::displayExit()
{

}

void Render::display()
{
	displayGame();
	SDL_Flip(m_pScreen);
}

void Render::draw(std::string _image, int _x, int _y)
{
	SDL_Rect offset;
	offset.x = _x;
	offset.y = _y;

	SDL_BlitSurface(m_image[_image], NULL, m_pScreen, &offset);
}

unsigned long Color(int R, int G, int B)
{
	//Convertit RGB en long
	return 65536*R + 256*G + B;
}

void Render::PutPixel(int x, int y, unsigned long pixel)
{
	if(x >= 0 && x < SIZE_SCREEN_X && y >= 0 &&  y < SIZE_SCREEN_Y)
	{
		int bpp = m_pScreen->format->BytesPerPixel;
		if(x < SIZE_SCREEN_X && y < SIZE_SCREEN_Y)
		{
			unsigned char *p = (unsigned char *)m_pScreen->pixels + y * m_pScreen->pitch + x * bpp;
			if (bpp==4)
				*(unsigned long*)p = pixel;
		}
	}
}

void Render::drawCircle(int rayon, int x_centre, int y_centre, int R, int G, int B)
{
	unsigned long color = Color(R, G, B);
	int x, y, m;
	x  = 0;
	y= rayon ;        // on se place en haut du cercle 
	m = 5 - 4 *rayon ;             // initialisation
	while( x <= y )      // tant qu'on est dans le second octant
	{
		PutPixel( x+x_centre, y+y_centre, color) ;
		PutPixel( y+x_centre, x+y_centre, color ) ;
		PutPixel( -x+x_centre, y+y_centre, color ) ;
		PutPixel( -y+x_centre, x+y_centre, color ) ;
		PutPixel( x+x_centre, -y+y_centre, color ) ;
		PutPixel( y+x_centre, -x+y_centre, color ) ;
		PutPixel( -x+x_centre, -y+y_centre, color ) ;
		PutPixel( -y+x_centre, -x+y_centre, color ) ;
		if( m > 0 )
		{//choix du point F
			y --;
			m = m - 8 * y ;
		}
		x ++;
		m += 8 * x + 4 ;
	}
}
void Render::drawLine(int x1, int y1, int x2, int y2, int R, int G, int B)  // Bresenham
{
	unsigned long color = Color(R, G, B);
	int x,y;
	int Dx,Dy;
	int xincr,yincr;
	int erreur;
	int i;

	Dx = abs(x2-x1);
	Dy = abs(y2-y1);
	if(x1<x2)
		xincr = 1;
	else
		xincr = -1;
	if(y1<y2)
		yincr = 1;
	else			
		yincr = -1;

	x = x1;
	y = y1;
	if(Dx>Dy)
	{
		erreur = Dx/2;
		for(i=0;i<Dx;i++)
		{
			x += xincr;
			erreur += Dy;
			if(erreur>Dx)
			{
				erreur -= Dx;
				y += yincr;
			}
			PutPixel(x, y,color);
		}
	}
	else
	{
		erreur = Dy/2;
		for(i=0;i<Dy;i++)
		{
			y += yincr;
			erreur += Dx;

			if(erreur>Dy)
			{
				erreur -= Dy;
				x += xincr;
			}
			PutPixel(x, y,color);
		}
	}
}

void Render::drawText(std::string _text, int _x, int _y)
{
	SDL_Surface* text = TTF_RenderText_Blended(m_font, _text.c_str(), m_textColor);
	SDL_Rect offset;
	offset.x = _x;
	offset.y = _y;
	SDL_BlitSurface(text, NULL, m_pScreen, &offset);
	SDL_FreeSurface(text);
}

void Render::drawEntities()
{
	int j = 0;
	for(std::vector<Entity*>::iterator i = m_entitiesToRender->begin(); i != m_entitiesToRender->end() && j < m_input->numberEntity; i++, j++)
	{
		drawEntity(*i, 0, 125, 255, 0, 255, 255);
		if (m_input->drawVelocity) 
			drawLine(	(int)(*i)->getPosition().x, (int)(*i)->getPosition().y, 
						(int)((*i)->getPosition().x + (*i)->getVelocity().x), (int)((*i)->getPosition().y + (*i)->getVelocity().y), 
						0, 255, 0);
		if (m_input->drawForce) 
			drawLine(	(int)(*i)->getPosition().x, (int)(*i)->getPosition().y, 
						(int)((*i)->getPosition().x + (*i)->getLastForce().x), (int)((*i)->getPosition().y + (*i)->getLastForce().y), 
						0, 0, 255);
	}
}

void Render::drawEntity(Entity* ent, int R, int G, int B, int Rext, int Gext, int Bext)
{
	for (int j = 6; j > 0; j--)
		drawCircle(j, (int)(ent)->getPosition().x, (int)(ent)->getPosition().y, R, G, B);
	drawCircle(7, (int)(ent)->getPosition().x, (int)(ent)->getPosition().y, Rext, Gext, Bext);
}

void Render::drawObstacles()
{
	for(std::vector<Obstacle*>::iterator i = m_obstaclesToRender->begin(); i != m_obstaclesToRender->end(); i++)
	{
		drawCircle((int)(*i)->m_radius, (int)(*i)->m_position.x, (int)(*i)->m_position.y, 255, 0, 0);
	}
}

void Render::displayGame()
{
	SDL_FillRect(m_pScreen, NULL, Color(120, 110, 100));
	
	drawEntities();

	//draw specific to mode
	switch(m_input->state)
	{
	case SEEK_FLEE:
		drawText("SEEK / FLEE", 30, 30);
		break;
	case PURSUIT_EVASION:
		drawText("PURSUIT / AVOID", 30, 30);
		break;
	case SEPARATION:
		drawText("SEPARATION", 30, 30);
		break;
	case ARRIVAL:
		drawText("ARRIVAL / OBSTACLES AVOIDANCE", 30, 30);
		drawObstacles();
		break;
	case WANDER:
		if (m_input->drawWanderCircle)
		{
			int j = 0;
			for (std::vector<Entity*>::iterator i = m_entitiesToRender->begin(); i != m_entitiesToRender->end() && j<m_input->numberEntity; i++, j++)
			{
				Vector2 direction = (*i)->getVelocity();
				direction.Normalize();
				Vector2 center = direction * ((*i)->getMaxForce() - 40.0f);

				//drawCircle(5,(*i)->getPosition().x + (*i)->getLastForce().x,(*i)->getPosition().y + (*i)->getLastForce().y, 0, 255, 0);
				drawCircle(5, (int)((*i)->getPosition().x + center.x + (*i)->getLastR().x), (int)((*i)->getPosition().y + center.y + (*i)->getLastR().y), 0, 255, 0);
				drawCircle(40, (int)((*i)->getPosition().x + center.x), (int)((*i)->getPosition().y + center.y), 255, 0, 0);
			}
		}
		drawText("WANDER", 30, 30);
		break;
	case PATHFOLLOWING:
		drawText("PATH FOLLOWING", 30, 30);
		drawObstacles();
		{
			int j = 0;
			for (std::vector<Entity*>::iterator i = m_entitiesToRender->begin(); i != m_entitiesToRender->end() && j<m_input->numberEntity; i++, j++)
			{
				Entity* p = *i;
				if (p->pTarget)
				{
					drawCircle(5, (int)p->pTarget->m_position.x, (int)p->pTarget->m_position.y, 255, 0, 0);
				}
			}
		}
		break;
	case UCA:
		drawText("UNALIGNED COLLISION AVOIDANCE", 30, 30);
		break;
	case COHESION:
		drawText("COHESION", 30, 30);
		break;
	case ALIGNMENT:
		drawText("ALIGNMENT", 30, 30);
		break;
	case FLOCKING:
		drawText("FLOCKING", 30, 30);
		break;
	case LEAD_FOLLOWING:
		drawText("LEAD_FOLLOWING", 30, 30);
		break;
	case SWARMING:
		drawText("SWARMING", 30, 30);
		break;
	case FORMATION_L:
		drawText("FORMATION_L", 30, 30);
		break;
	case FORMATION_V:
		drawText("FORMATION_V", 30, 30);
		break;
	case FORMATION_CIRCLE:
		drawText("FORMATION_CIRCLE", 30, 30);
		break;
	case FORMATION_2LEVEL:
		drawText("FORMATION_2LEVEL", 30, 30);
		drawObstacles();
		break;
	case FORMATION_OF_FORMATIONS:
		drawText("FORMATION_OF_FORMATIONS", 30, 30);
		break;
	case FORMATION_DYNAMIC:
		drawText("FORMATION_DYNAMIC", 30, 30);
		break;
	}

}



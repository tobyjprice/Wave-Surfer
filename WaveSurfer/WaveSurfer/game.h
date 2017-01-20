#pragma once
#include "SDL.h"
#include "Sprite.h"

class game
{
public:
	game();
	game(SDL_Window* window, SDL_Renderer* renderer);
	~game();

	void load_Surfaces();
	void update(double deltaTime);
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
	Sprite* sprite;
	Sprite* seagull;
private:
	SDL_Surface* playerSurf;
	SDL_Surface* seagullSurf;
};


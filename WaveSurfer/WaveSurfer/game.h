#pragma once

#include "SDL.h"
#include "Sprite.h"
#include <vector>

class game
{
public:
	game();
	game(SDL_Window* window, SDL_Renderer* renderer);
	~game();

	void load_Surfaces();
	void update(double deltaTime);
	void updateBg(double deltaTime);

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
	Sprite* sprite;
	Sprite* seagull;
	std::vector<Sprite*> bg;
private:
	SDL_Surface* playerSurf;
	SDL_Surface* seagullSurf;
	SDL_Surface* bgSurf;
	
};


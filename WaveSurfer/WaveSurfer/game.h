#pragma once

#include "SDL.h"
#include "Sprite.h"
#include "ObstSpawner.h"
#include <vector>
#include <iostream>

class game
{
public:
	game();
	game(SDL_Window* window, SDL_Renderer* renderer);
	~game();

	void load_Surfaces();
	void update(double deltaTime, SDL_GameController *currentController);
	void updateBg(double deltaTime);

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;

	ObstSpawner* obSpawner;
	Sprite* sprite;
	Sprite* cloud;
	Sprite* sea;
	Wave* waves;
	std::vector<Sprite*> bg;
	std::vector<Sprite*> spriteList;

	std::vector<Sprite*> pixelList;
private:
	SDL_Surface* playerSurf;
	SDL_Surface* seagullSurf;
	SDL_Surface* bgSurf;
	SDL_Surface* cloudSurf;
	SDL_Surface* pixelSurf;
	SDL_Surface* seaSurf;

};
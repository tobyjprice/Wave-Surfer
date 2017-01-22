#pragma once

#include "SDL.h"
#include "Sprite.h"
#include "Text.h"
#include "ObstSpawner.h"
#include <vector>
#include <iostream>
#include <string>

class game
{
public:
	game();
	game(SDL_Window* window, SDL_Renderer* renderer);
	~game();

	void load_Surfaces();
	void update(double deltaTime, SDL_GameController *currentController);
	void updateBg(double deltaTime);
	bool checkLandAngle();

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;

	ObstSpawner* obSpawner;
	Sprite* sprite;
	Sprite* cloud;
	Sprite* sea;
	Text* scoreText;
	Wave* waves;
	double score;
	int startTime;

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
	TTF_Font* scoreFont;

};
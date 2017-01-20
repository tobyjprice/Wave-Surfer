#include "game.h"



game::game()
{

}

game::game(SDL_Window* inWindow, SDL_Renderer* inRenderer)
{
	SDL_Log("Game initialised");
	window = inWindow;
	renderer = inRenderer;
	int* rendW = new int(0);
	int* rendH = new int(0);
	SDL_RenderGetLogicalSize(renderer, rendW, rendH);

	load_Surfaces();

	if (window != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Window Initialised OK!\n");
	}

	if (renderer != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Window Initialised OK!\n");
	}


	Sprite* temp = new Sprite(320, 720, 0, -540, renderer, -1, bgSurf, 0.033);
	bg.emplace_back(temp);
	Sprite* temp2 = new Sprite(320, 720, 0, -1260, renderer, -1, bgSurf, 0.033);
	bg.emplace_back(temp2);
	
	sprite = new Sprite(64, 64, (*rendW / 2) - 32, (*rendH / 2) - 32, renderer, 1, playerSurf, 0.1);
	seagull = new Sprite(32, 32, 10, 10, renderer, 0, seagullSurf, 0.023);
}

void game::update(double dt)
{
	sprite->update(dt);
	seagull->update(dt);
	updateBg(dt);
}

void game::updateBg(double dt)
{
	for (auto& back : bg)
	{
		if (back->animAcc > back->animDur)
		{
			back->dstRect.y += 1;

			if (back->dstRect.y >= 180)
			{
				back->dstRect.y = -1260;
			}

			back->animAcc = 0;
		}
		back->animAcc += dt;
	}
}

void game::load_Surfaces()
{
	playerSurf = IMG_Load("..//resources//jesus.png");
	if (!playerSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	seagullSurf = IMG_Load("..//resources//seagull.png");
	if (!seagullSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	bgSurf = IMG_Load("..//resources//background.png");
	if (!seagullSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}
}

game::~game()
{
	
}

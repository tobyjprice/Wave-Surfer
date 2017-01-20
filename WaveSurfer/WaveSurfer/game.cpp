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


	Sprite* temp = new Sprite(640, 1440, 0, -1080, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp);
	Sprite* temp2 = new Sprite(640, 1440, 0, -2520, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp2);
	
	sprite = new Sprite(64, 64, (*rendW / 2) - 32, (*rendH / 2) - 32, renderer, 1, playerSurf, 0.1, 64);
	spriteList.push_back(sprite);
	seagull = new Sprite(32, 32, 320, 180, renderer, 0, seagullSurf, 0.023, 32);
	spriteList.push_back(seagull);

	cloud = new Sprite(128, 64, 100, 30, renderer, -1, cloudSurf, 0.023, 64);
	spriteList.push_back(cloud);
}

void game::update(double dt)
{
	sprite->update(dt);
	seagull->update(dt);
	updateBg(dt);

	sprite->yPos = sprite->yPos + (sprite->yVel * dt);
	sprite->yVel += 10 * dt;

	sprite->dstRect.y = sprite->yPos;
}

void game::updateBg(double dt)
{
	for (auto& back : bg)
	{
		if (back->animAcc > back->animDur)
		{
			back->dstRect.y += 1;

			if (back->dstRect.y >= 360)
			{
				back->dstRect.y = -2520;
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
	if (!bgSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	cloudSurf = IMG_Load("..//resources//cloud.png");
	if (!cloudSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}
}

game::~game()
{
	
}

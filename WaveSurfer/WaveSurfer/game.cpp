#include "game.h"
#include <math.h>


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
	
	sprite = new Sprite(64, 64, (*rendW / 2) - 32, (*rendH / 2) - 32, renderer, 1, playerSurf, 0.05, 64);
	spriteList.push_back(sprite);
	seagull = new Sprite(32, 32, 320, 180, renderer, 0, seagullSurf, 0.023, 32);
	spriteList.push_back(seagull);
	waves = new Wave(32, 32, 320, 180, renderer, 0, seagullSurf, 0.023, 32);
	waves->xPos = 0;

	cloud = new Sprite(128, 64, 100, 30, renderer, -1, cloudSurf, 0.023, 64);
	spriteList.push_back(cloud);

	for (int x = 0; x < 500; x++)
	{
		Sprite* tempPx = new Sprite(1, 1, 0, 0, renderer, -1, pixelSurf, 0.023, 1);
		pixelList.push_back(tempPx);
	}
}

void game::update(double dt, SDL_GameController *currentController)
{
	sprite->oldX = waves->xPos;
	waves->updatePos(currentController);

	sprite->update(dt);
	seagull->update(dt);
	updateBg(dt);

	//Figure out current wave
	//std::cout << waves->xPos << " - " << -pow((waves->xPos), 3) * 0.1 << std::endl;

	// Calculate angles
	//double waveAng = atan( (waves->xPos - sprite->oldX) / (60 * cos(waves->xPos) + 200 - sprite->oldY) );

	//double angleDiff = waves->oldAng - waveAng;

	//sprite->rotation = waveAng * (180/ M_PI);
	//std::cout << sprite->rotation << std::endl;

	//sprite->rotation += angleDiff * (180 / M_PI);

	//std::cout << angleDiff << std::endl;

	//Ground collision code for cosine wave
	//if (sprite->yPos >= (float)60 * cos(waves->xPos) + (float)200) {
		//If sprite is under ground place on ground using velocity
		float fryPos = (float)60 * cos(waves->xPos) + (float)200;

		std::cout << "guff" << std::endl;

		double followGravity = sprite->yVel + 5 * dt;
		double followLine = -(sprite->yPos - fryPos);

		if (followGravity < followLine) {
			//Follow gravity
			sprite->yVel = followGravity;
		}
		else {
			//Follow the line
			sprite->yVel = followLine;
		}

		//if () {
		//	sprite->yVel = -(sprite->yPos - fryPos);
		//	
		//}
		//else {
		//	//If sprite is above ground apply gravity
		//	sprite->yVel += 5 * dt;

		//	std::cout << "other guff" << std::endl;
		//}
		
		std::cout << sprite->yPos << "\t" << sprite->yVel << std::endl;

	//}
	//else {
	//	//If sprite is above ground apply gravity
	//	sprite->yVel += 5* dt;
	//} 

	for (int x = 0; x < 500; x++)
	{
		pixelList[x]->dstRect.x = x + waves->xPos;
		pixelList[x]->dstRect.y = (float)60 * cos(waves->xPos) + (float)200;
	}

	

	
	
	//#####################

	/*Ground collision code for other wave
	if (sprite->yPos >= -pow((waves->xPos), 3) * 0.1) {
		//If sprite is under ground place on ground using velocity
		sprite->yPos = -pow( (waves->xPos) , 3) * 0.1;
		sprite->yVel = (oldY - sprite->yPos) * dt;
	}
	else {
		//If sprite is above ground apply gravity
		sprite->yVel += 10 * dt;
	}*/

	//#####################
	
	//std::cout << sprite->yVel << std::endl;

	sprite->oldY = sprite->yPos;

	sprite->yPos = sprite->yPos + sprite->yVel;

	sprite->dstRect.y = sprite->yPos;

	

	waves->dstRect.x = waves->xPos;


	
	//waves->oldAng = waveAng;
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

	pixelSurf = IMG_Load("..//resources//pixel.png");
	if (!pixelSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}
}

game::~game()
{
	
}



void Wave::updatePos(SDL_GameController *currentController) {
	xPos = xPos + (SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_LEFTY) * 0.000003);
	//std::cout << xPos << std::endl;
}
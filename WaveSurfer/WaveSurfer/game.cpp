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

	score = 0;
	startTime = SDL_GetTicks();

	obSpawner = new ObstSpawner(renderer, seagullSurf);

	Sprite* temp = new Sprite(1024, 2304, 0, -1728, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp);
	Sprite* temp2 = new Sprite(1024, 2304, 0, -4032, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp2);
	
	waves = new Wave(1875, 202, 0, 390, renderer, 0, seaSurf, 0.023, 64);
	waves->xPos = 0;

	sprite = new Sprite(64, 64, 32, 0, renderer, 1, playerSurf, 0.05, 64);
	spriteList.push_back(sprite);

	cloud = new Sprite(128, 64, 100, 30, renderer, -1, cloudSurf, 0.023, 64);
	spriteList.push_back(cloud);

	scoreText = new Text("Hello", *rendW - 110, *rendH-60, 100, 50, renderer, scoreFont);

	for (int x = 0; x < 500; x++)
	{
		Sprite* tempPx = new Sprite(1, 1, 0, 0, renderer, -1, pixelSurf, 0.023, 1);
		pixelList.push_back(tempPx);
	}
}

void game::update(double dt, SDL_GameController* currentController)
{
	for (auto& enemy : obSpawner->enemyList)
	{
		if (sqrt(pow((sprite->dstRect.x + (sprite->dstRect.w / 2) )  - (enemy->dstRect.x + (enemy->dstRect.w / 2)),2) + pow((sprite->dstRect.y + (sprite->dstRect.h / 2)) - (enemy->dstRect.y + (enemy->dstRect.h / 4)), 2)) < (sprite->dstRect.w / 2) + (enemy->dstRect.h / 4))
		{
			std::cout << "COLLISION";
			//running = false;
		}
	}

	// Score over time needs work!

	//score += dt * 100;
	
	//std::cout << "RIGHT TRIGGER" << rTrig << std::endl;

	//std::cout << "Score: " << score << std::endl;

	sprite->oldX = waves->xPos;
	waves->updatePos(currentController, dt);

	sprite->update(dt);
	updateBg(dt);

	// Rotation calculations
	double rTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	double lTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

	if (sprite->rotAcc < 50000 && sprite->rotAcc > -50000)
	{
		sprite->rotAcc += (rTrig - lTrig) * 0.05;

	}

	sprite->rotVel = sprite->rotAcc * dt;
	sprite->rotation += sprite->rotVel * dt;

	sprite->flipAcc += sprite->rotVel * dt;

	if (sprite->rotation > 360)
	{
		std::cout << "FLIP" << std::endl;
		sprite->rotation -= 360;
	}
	else if (sprite->rotation < 0)
	{
		std::cout << "FLIP" << std::endl;
		sprite->rotation += 360;
	}


	if (sprite->rotAcc > 0 && rTrig == 0)
	{
		sprite->rotAcc -= 1250;
	}
	else if (sprite->rotAcc < 0 && lTrig == 0)
	{			
		sprite->rotAcc += 1250;
	}
	else
	{
		//sprite->rotAcc = 0;
	}



	double axis = 0;
	double inAxis = waves->updatePos(currentController, dt);

	//Updating position of the waves
	if (inAxis > 0.04) { inAxis = 0.04; };
	if (inAxis < -0.04) { inAxis = -0.04; };

	axis = ((inAxis + waves->lastInp * 30) / 31 + waves->xPos);
	waves->xPos = axis - 0.042;
	waves->lastInp = (inAxis + waves->lastInp * 30) / 31;

	double waveVel = (waves->oldX - waves->xPos) / dt;
	obSpawner->update(dt, waveVel/2);

	float fryPos = (float)60 * cos(waves->xPos + M_PI + 1.1) + 400;

	double followGravity = sprite->yVel + 5 * dt;
	double followLine = -(sprite->yPos - fryPos);

	if (followGravity < followLine) {
		//Follow gravity

		sprite->yVel = followGravity * 1.005;

		
	}
	else {
		//Follow the line
		sprite->yVel = followLine * 1.005;


		if (!checkLandAngle())
		{
			sprite->yPos -= 300;
		}
		else
		{
			int numOfFlips = abs(sprite->flipAcc / 360);
			std::cout << score << std::endl;
			score += 100 * numOfFlips;
		}

		
		sprite->flipAcc = 0;

		
	}

	/*for (int x = 0; x < 500; x++)
	{
		pixelList[x]->dstRect.x = x + waves->xPos;
		pixelList[x]->dstRect.y = 60 * cos(waves->xPos) + 200;
	}*/

	sprite->oldY = sprite->yPos;
	sprite->yPos = sprite->yPos + sprite->yVel;
	sprite->dstRect.y = sprite->yPos;

	if (waves->dstRect.x < -704)
	{
		waves->xPos = 0;
	}

	std::string s = std::to_string(score);
	char const *pchar = s.c_str();
	scoreText->update_texture(pchar);

	waves->dstRect.x = waves->xPos * 60 -330;
	waves->oldX = waves->xPos;
}

void game::updateBg(double dt)
{
	for (auto& back : bg)
	{
		if (back->animAcc > back->animDur)
		{
			back->dstRect.y += 1;

			if (back->dstRect.y >= 576)
			{
				back->dstRect.y = -4032;
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

	seaSurf = IMG_Load("..//resources//sea.png");
	if (!seaSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	scoreFont = TTF_OpenFont("..//resources//true-crimes.ttf", 28);
	if (!scoreFont)
	{
		SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
	}
}

bool game::checkLandAngle()
{
	double x1, x2, y1, y2;

	x1 = waves->xPos;
	x2 = waves->xPos + sprite->dstRect.w;

	y1 = (float)60 * cos(x1 + M_PI + 1.1) + 400;
	y2 = (float)60 * cos(x2 + M_PI + 1.1) + 400;

	double angle = ( atan((y1 - y2) / (x2 - x1)) ) * (180/M_PI);

	if (sprite->rotation < 1 || sprite->rotation > 359)
	{
		return true;
	}
	else if (angle > -46 && angle < 0)
	{
		if (sprite->rotation > 280 || (sprite->rotation < 50 && sprite->rotation > 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (angle > 0 && angle < 46)
	{
		if ((sprite->rotation < 90 && sprite->rotation > 0) || sprite->rotation > 300)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

game::~game()
{
	
}



double Wave::updatePos(SDL_GameController *currentController, double dt) {
	return (SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_LEFTY) * 0.0001) * dt;
}
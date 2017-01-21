#include <iostream>
#include "SDL.h"
#include "game.h"
#include <chrono>

static void update(game* game, double deltaTime, SDL_GameController *currentController);
static void process_input(game* game);
static void render(game* game);

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		SDL_Log("SDL Initialised OK!\n");
	}

	SDL_DisplayMode display;
	SDL_GetDesktopDisplayMode(0, &display);

	SDL_Window* window = SDL_CreateWindow("Wave Surfer", display.w / 4, display.h /4, display.w / 2, display.h / 2, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);

	// MAGIC CODE IN HERE THANKS GOODBYE
	SDL_RenderSetLogicalSize(renderer, 640, 360);

	game game(window, renderer);
	game.running = true;

	/*std::chrono::high_resolution_clock::time_point prevTime = std::chrono::high_resolution_clock::now();
	double acc = 0.0;
	double dt = 0.00025;
	double t = 0.0;
	int frameCounter = 0;*/

	//Controller bindings
	SDL_GameController *controllers[8];

	//Finds all connected controllers and adds them to the above array
	int controllerNum = 0;
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (i >= 8) {
			break;
		}
		if (SDL_IsGameController(i)) {
			controllers[controllerNum] = SDL_GameControllerOpen(i);
			controllerNum++;
		}
	}

	//If no controllers are detected set first controller to null pointer to allow other bits of code to function gracefully
	if (controllerNum == 0) {
		controllers[controllerNum] = nullptr;
	}

	int start = SDL_GetTicks();
	int time = 0;
	int dt = 0;

	while (game.running)
	{
		/*auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - prevTime);
		double frameTimeNano = deltaTime.count();
		double frameTimeSec = frameTimeNano / 1000000000;
		prevTime = currTime;

		acc += frameTimeSec;*/

		//##### GAME LOOP #####

		start = SDL_GetTicks();

		double actualDT = dt / (float)1000;

		std::cout << actualDT << std::endl;

		process_input(&game);
		update(&game, actualDT, controllers[0]);
		render(&game);

		time = SDL_GetTicks() - start;

		if (time < 0) continue;

		int sleepTime = 16 - time;

		if (sleepTime > 0)
		{
			SDL_Delay(sleepTime);
		}

		dt = (SDL_GetTicks() - start);

		

		//#####################

		/*if (frameCounter > 100)
		{
			SDL_Log("FPS: %s \n", frameTimeSec);
			frameCounter = 0;
		}
		else
		{
			frameCounter++;
		}*/
	}

	return 0;
}

void update(game* game, double dt, SDL_GameController *currentController)
{
	game->update(dt, currentController);
}

void process_input(game* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				game->running = false;
				return;
			default:
				break;
			}
		default:
			break;
		}
	}
}

void render(game* game)
{
	SDL_RenderClear(game->renderer);
	for (auto& bg : game->bg)
	{
		SDL_RenderCopy(game->renderer, bg->texture, NULL, &bg->dstRect);
	}
	for (auto& sprite : game->spriteList)
	{
		SDL_RenderCopyEx(game->renderer, sprite->texture, &sprite->srcRect, &sprite->dstRect, sprite->rotation, 0, SDL_FLIP_NONE);
	}

	SDL_RenderPresent(game->renderer);
}
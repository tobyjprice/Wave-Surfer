#include <iostream>
#include "SDL.h"
#include "game.h"
#include <chrono>

static void update(game* game, double deltaTime);
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

	SDL_RenderSetLogicalSize(renderer, 320, 180);

	game game(window, renderer);
	game.running = true;

	std::chrono::high_resolution_clock::time_point prevTime = std::chrono::high_resolution_clock::now();
	double acc = 0.0;
	double dt = 0.00025;
	double t = 0.0;
	int frameCounter = 0;

	while (game.running)
	{
		auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - prevTime);
		double frameTimeNano = deltaTime.count();
		double frameTimeSec = frameTimeNano / 1000000000;
		prevTime = currTime;

		acc += frameTimeSec;

		while (acc >= dt)
		{
			update(&game, dt);
			acc -= dt;
			t += dt;
		}

		process_input(&game);
		render(&game);

		if (frameCounter > 100)
		{
			//SDL_Log("FPS: %s \n", frameTimeSec);
			frameCounter = 0;
		}
		else
		{
			frameCounter++;
		}
	}

	return 0;
}

void update(game* game, double dt)
{
	game->update(dt);
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
				break;
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
	SDL_RenderCopy(game->renderer, game->sprite->texture, &game->sprite->srcRect, &game->sprite->dstRect);
	SDL_RenderCopy(game->renderer, game->seagull->texture, &game->seagull->srcRect, &game->seagull->dstRect);
	
	SDL_RenderPresent(game->renderer);
}
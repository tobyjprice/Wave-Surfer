#pragma once
#include "SDL.h"
#include "SDL_image.h"

class Sprite
{
public:
	Sprite();
	Sprite(int w, int h, double x, double y, SDL_Renderer* renderer, int animStyle, SDL_Surface* surface, double animDur, int srcWidth);
	~Sprite();

	void update(double deltaTime);
	void animateForward(double deltaTime);
	void animatePingPong(double deltaTime);

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect srcRect, dstRect;
	double animAcc, animDur;
	int animStyle;
	bool animDir;
	double xPos, yPos, xVel, yVel;
	int srcWidth;
	double oldY, oldX;
	double rotation;
};

class Wave : Sprite {
public:
	double xPos;
	double oldAng;

	void updatePos(SDL_GameController *currentController);
};

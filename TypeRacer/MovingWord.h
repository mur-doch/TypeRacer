#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class MovingWord
{
public:
	//Initializes variables
	MovingWord(std::string word, int x, int y);

	//Deallocates memory
	~MovingWord();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	bool loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, TTF_Font* font, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();


	std::string word;

	int mX;
	int mY;

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};


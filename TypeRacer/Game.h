#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <time.h>

#include "TextureWrapper.h"
#include "MovingWord.h"

class Game
{
public:
	Game();
	~Game();
	void setRenderer(SDL_Renderer* renderer);
	bool loadWordList();
	bool loadMedia();
	bool handleEvents();
	void update();
	void render();

	int checkWord();

	SDL_Renderer* renderer;
	SDL_Event e;
	SDL_Color textColor;
	TTF_Font* font;
	TextureWrapper typingTexture;

	Uint32 currentTime;
	Uint32 lastUpdate;

	Uint32 timeSinceLastWord;
	Uint32 timeForWordSpawn;

	std::vector<std::string> wordlist;
	std::vector<std::string> availableWords;
	std::vector<MovingWord*> words;
	int wordSpeed;

	std::string buffer;
	bool updateTyping;
};


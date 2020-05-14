#include "Game.h"

Game::Game()
{
	renderer = NULL;
	font = NULL;
	updateTyping = false;
	textColor = { 0, 0, 0 };
	buffer = "";

	currentTime = 0;
	lastUpdate = 0;

	timeSinceLastWord = 0;
	timeForWordSpawn = 2000;
	wordSpeed = 1;

	srand(time(NULL));
}

Game::~Game()
{
	for (int i = 0; i < words.size(); i++)
	{
		words[i]->free();
		delete words[i];
	}
}

void Game::setRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

bool Game::loadWordList()
{
	std::string line;
	std::ifstream rwords("wordlist.txt");

	while (getline(rwords, line))
	{
		wordlist.push_back(line);
		availableWords.push_back(line);
	}

	return true;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	font = TTF_OpenFont("fonts/arial.ttf", 24);
	if (font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		if (!loadWordList())
		{
			success = false;
		}
	}

	return success;
}

bool Game::handleEvents()
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			return true;
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			buffer += e.text.text;
			updateTyping = true;
		}
		
		if (e.type == SDL_KEYDOWN) 
		{
			SDL_Keycode k = e.key.keysym.sym;
			int wordIndex = -1;
			switch (k)
			{
			case SDLK_ESCAPE:
				return true;
				break;
			case SDLK_RETURN:
				wordIndex = checkWord();
				if (wordIndex != -1)
				{
					printf("Got word %d!\n", wordIndex);
					words[wordIndex]->free();
					delete words[wordIndex];
					words.erase(words.begin() + wordIndex);
					buffer = "";
					updateTyping = true;
				}
				break;
			case SDLK_BACKSPACE:
				if (buffer != "")
				{
					buffer.pop_back();
				}
				updateTyping = true;
			}
		}
	}

	return false;
}

void Game::update()
{
	currentTime = SDL_GetTicks();
	Uint32 delta = currentTime - lastUpdate;
	lastUpdate = currentTime;

	timeSinceLastWord += delta;
	printf("%d %d\n", timeSinceLastWord, timeForWordSpawn);
	if (timeSinceLastWord >= timeForWordSpawn && availableWords.size() > 0)
	{
		// add new word
		int wordIndex = std::rand() % availableWords.size();
		int yPos = std::rand() % 425;

		MovingWord* newText = new MovingWord(availableWords[wordIndex] , 0, yPos);
		newText->loadFromRenderedText(renderer, availableWords[wordIndex].c_str(), font, textColor);
		words.push_back(newText);

		availableWords.erase(availableWords.begin() + wordIndex);
		timeSinceLastWord = 0;
	}

	for (int i = 0; i < words.size(); i++)
	{
		words[i]->mX += wordSpeed;
	}
}

void Game::render()
{
	int bufferLen = buffer.length();
	if (updateTyping && bufferLen != 0) // could do buffer != ""
	{
		typingTexture.loadFromRenderedText(renderer, buffer.c_str(), font, textColor);
		updateTyping = false;
	}

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	for (int i = 0; i < words.size(); i++)
	{
		words[i]->render(renderer, 0, 0);
	}

	if (bufferLen != 0)
	{
		typingTexture.render(renderer, 0, 450);
	}

	SDL_RenderPresent(renderer);
}

int Game::checkWord()
{
	if (buffer.length() == 0)
	{
		return -1;
	}

	for (int i = 0; i < words.size(); i++)
	{
		if (words[i]->word == buffer)
		{
			return i;
		}
	}

	return -1;
}
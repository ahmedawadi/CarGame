#pragma once
#include <iostream>
#include<SFML/Graphics.hpp>
#include "Game.h"

class Game;

class Car
{
	sf::Texture carImage;
	sf::Sprite *carSprite;
	std::string path;
	int streetNumber = 3;
	float gameWidth;
	float gameHeight;
	float streetWidth;
	friend class Game;
public:
	Car(std::string path, float gameWidth, float gameHeight, float streetWidth);
	~Car();
	sf::Sprite* drive();
	void moveRigt();
	void moveLeft();
	std::string getPath();
	
};


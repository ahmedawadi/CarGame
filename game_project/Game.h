#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "obstacles.h"


class Obstacle;
class Car;

class Game
{
	sf::Texture image;
	sf::Texture loseImage;
	sf::Sprite bgImageGame;//the background of our game
	sf::Sprite loseSprite;
	sf::RenderWindow* window;
	sf::Text *scoreText;
	int streetWidth = 140;
	float gameHeight;
	float gameWidth;
	float scrolledYPosition = 0;
	int score;
	friend class Car;
	friend class Obstacle;
	friend class Obstacles;
	void drawScore();
	void startInterface(std::string& selectedPath);
	void lose(Obstacles& obstacles);
	void makeItHarder(Obstacles& obstacles);
	void DecrementerInterface(int decrementor);
	void moveCar();
	void chooseCar(std::string& selectedPath);
	void destroyCars();
public:
	Game(std::string gameName, float gameHeight=0, float gameWidth=0);
	Game();
	~Game();
	bool createImage(std::string path, float xPostion=0, float yPosition=0);
	void play();
	


};


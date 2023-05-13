#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

class Game;

class Obstacle
{	
	sf::Texture obstaclePic;
	sf::Sprite obstacleSprite;
	int objectAppearance = 0;
	int xPosition;
	int streetNumber;
	float yPosition = -800;
	Game& game;
	friend class Game;
	friend class Obstacles;
public:
	Obstacle(Game& game);
	void createObstacle(int streetNumber);
	void updateObstacle(int streetNumber);
	void moveObstacle(float speed, int obstacleNumber, int previousNumber);
};


#pragma once
#include <list>
#include "SFML/Graphics.hpp"
#include "Obstacle.h"

class Game;
class Obstacle;

class Obstacles 
{
	std::list<Obstacle*> obstacles;
	Game& game;
	friend class Game;
public:
	Obstacles(Game& game);
	~Obstacles();
	void createNewObstacle();//add a new obstacle to the game
	void updateObstacles();//update the obstacles for a new game
	void moveObstacles();
	bool checkLose(sf::Sprite* carSprite);
	void drawObstacles();
	int getSize();
	static int getStreetNumber(int previousNumber, bool previous = false);
};


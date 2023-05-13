#include "Obstacles.h"
#include <cstdlib>

Obstacles::Obstacles(Game& game):game(game){
	Obstacle* obstacle = new Obstacle(game);
	obstacles.push_back(obstacle);
	
	srand(time(NULL));
	int street = 1 + rand() % 4;

	obstacles.back()->createObstacle(street);

}

Obstacles::~Obstacles()
{
	for (auto obstacle = obstacles.begin(); obstacle != obstacles.end(); ++obstacle)
		delete *obstacle;
}

void Obstacles::createNewObstacle() {

	obstacles.push_back(new Obstacle(game));
	
	srand(time(NULL));
	int street = 1 + rand() % 4;

	obstacles.back()->createObstacle(getStreetNumber(obstacles.back()->streetNumber, true));
}

void Obstacles::updateObstacles()
{
	int street = -1;
	int size = obstacles.size();
	int previous = 0;//number of previous streetNumber created

	while (size > 1) {
		delete obstacles.back();
		obstacles.pop_back();
		size--;
	}

	obstacles.back()->updateObstacle(getStreetNumber(street));
}

void Obstacles::moveObstacles()
{
	int obstacleNumber = 1;
	Obstacle* previousObstacle = obstacles.back();//just to do not make it undefined

	for (auto ob = obstacles.begin(); ob != obstacles.end(); ++ob) {

		Obstacle* obstacle = *ob;
		
		if (game.score < 15)
			obstacle->moveObstacle(0.7f, obstacleNumber, previousObstacle->streetNumber);
		else if (game.score < 40)
			obstacle->moveObstacle(1.0f, obstacleNumber, previousObstacle->streetNumber);
		else if (game.score < 70)
			obstacle->moveObstacle(1.3f, obstacleNumber, previousObstacle->streetNumber);
		else if (game.score < 100)
			obstacle->moveObstacle(1.6f, obstacleNumber, previousObstacle->streetNumber);
		else if (game.score < 130)
			obstacle->moveObstacle(1.9f, obstacleNumber, previousObstacle->streetNumber);
		else
			obstacle->moveObstacle(2.2f, obstacleNumber, previousObstacle->streetNumber);
		
		obstacleNumber++;

		if (obstacleNumber != 1)
			previousObstacle = *ob;//get the previous obstacle to avoid the get of the same street number by the rand function 
	}
	obstacleNumber = 1;

}

bool Obstacles::checkLose(sf::Sprite *carSprite)
{
	for (auto ob = obstacles.begin(); ob != obstacles.end(); ++ob) {
		Obstacle* obstacle = *ob;
		if (abs(obstacle->obstacleSprite.getPosition().x - carSprite->getPosition().x) <= 50 && abs(obstacle->obstacleSprite.getPosition().y - carSprite->getPosition().y) <= 50)
			return true;
	}
	return false;
}

void Obstacles::drawObstacles()
{	
	for (auto ob = obstacles.begin(); ob != obstacles.end(); ++ob) {
		Obstacle* obstacle = *ob;
		game.window->draw(obstacle->obstacleSprite);
	}
	
}

int Obstacles::getSize()
{
	return obstacles.size();
}

int Obstacles::getStreetNumber(int previousNumber,bool previous) {

	srand(time(NULL));
	int street = rand() % 4 + 1;
	
	if (previous && street == previousNumber) {
		switch (street) {
			case 1:
				return rand() % 3 + 2;
			case 2:
				return rand() % 2 + 3;
			case 3:
				return rand() % 2 + 1;

			case 4:
				return rand() % 3 + 1;
		}
	}
}

#include "Obstacle.h"

Obstacle::Obstacle(Game& game) :game(game) {
	obstaclePic.loadFromFile("../pictures/obstacle.png");
	obstacleSprite.setTexture(obstaclePic);
}

void Obstacle::createObstacle(int streetNum)
{	
	streetNumber = streetNum;
	//resize the obstacle to fit the street
	float scaleX = ((game.gameWidth / obstaclePic.getSize().x) / 4);
	float scaleY = (game.gameHeight / obstaclePic.getSize().y)/4;

	obstacleSprite.setScale(scaleX, scaleY);

	switch (streetNumber) {
		case 1:
			xPosition = (game.gameWidth / 10) - 20;
			break;
		case 2:
			xPosition = (game.gameWidth / 10) - 20 + game.streetWidth;
			break;
		case 3:
			xPosition = (game.gameWidth / 10) - 20 + (2 * game.streetWidth);
			break;
		case 4:
			xPosition = (game.gameWidth / 10) - 20 + (3 * game.streetWidth);
			break;
	}

	obstacleSprite.setPosition(xPosition, yPosition);
}

void Obstacle::updateObstacle(int streetNum) {

	streetNumber = streetNum;
	//update the place of the obstacle from a street to another street 
	if (game.score == 0)
		yPosition = -800;
	else if (game.score < 5)
		yPosition = -400;
	else if (game.score < 10)
		yPosition = -200;
	else
		yPosition = -150;

	switch (streetNumber) {
		case 1:
			xPosition = (game.gameWidth / 10) - 20;
			break;
		case 2:
			xPosition = (game.gameWidth / 10) - 20 + game.streetWidth;
			break;
		case 3:
			xPosition = (game.gameWidth / 10) - 20 + (2 * game.streetWidth);
			break;
		case 4:
			xPosition = (game.gameWidth / 10) - 20 + (3 * game.streetWidth);
			break;
	}
}


void Obstacle::moveObstacle(float speed, int obstacleNumber, int previousNumber) {

	if (yPosition < game.gameHeight) {
		yPosition += speed;
		obstacleSprite.setPosition(xPosition, yPosition);
	}
	else {
		srand((unsigned)time(NULL));
		game.score++;
		int streetNumber = 1 + rand() % 4;
		
		if (obstacleNumber > 1)
		{
			streetNumber = Obstacles::getStreetNumber(previousNumber, true);
			updateObstacle(streetNumber);
		}
		else 
			updateObstacle(streetNumber);
	}

}



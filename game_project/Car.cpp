#include "Car.h"

Car::Car(std::string path,float gameWidth, float gameHeight, float streetWidth)
{
	this->gameWidth = gameWidth;
	this->gameHeight = gameHeight;
	this->streetWidth = streetWidth;
	carSprite = new sf::Sprite;
	this->path = path;
	carImage.loadFromFile(path);
	carSprite->setTexture(carImage);

}

Car::~Car() {
	delete carSprite;
}

sf::Sprite* Car::drive()
{
	//change the size of the image
	int xSize = carImage.getSize().x;
	int ySize = carImage.getSize().y;

	float xScale = gameWidth / (xSize * 4);
	float yScale = gameHeight / (ySize * 4);

	carSprite->setScale(xScale, yScale);
	carSprite->setPosition((gameWidth / 2) - 15, gameHeight / 2);
	return carSprite;

}

void Car::moveRigt()
{
	float xPosition = carSprite->getPosition().x;
	if (streetNumber < 4) {
		xPosition += streetWidth;
		carSprite->setPosition(xPosition, carSprite->getPosition().y);
		streetNumber++;
	}
}

void Car::moveLeft()
{
	float xPosition = carSprite->getPosition().x;
	if (streetNumber > 1) {
		xPosition -= streetWidth;
		carSprite->setPosition(xPosition, carSprite->getPosition().y);
		streetNumber--;
	}
}

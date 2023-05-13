#include <cmath>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include "Game.h"
#include "Car.h"


Game::Game(std::string gameName, float gameHeight, float gameWidth):gameWidth(gameWidth),gameHeight(gameHeight)
{
	window = new sf::RenderWindow(sf::VideoMode(gameWidth, gameHeight), gameName);
	scoreText = new sf::Text;
}

Game::Game()
{
	gameHeight = 0;
	score = 0;
	gameWidth = 0;
	window = new sf::RenderWindow(sf::VideoMode(gameWidth, gameHeight), "test");
}

Game::~Game()
{
	delete window;
	delete scoreText;
}

bool Game::createImage(std::string path, float xPosition, float yPosition)
{
	loseImage.loadFromFile("../pictures/lose.png");
	image.loadFromFile(path);

	/*get the scale to put the full image on the screen */
	int xSize = image.getSize().x;
	int ySize = image.getSize().y;
	float scaleX = gameWidth / xSize;
	float scaleY = gameHeight / ySize;

	bgImageGame.setTexture(image);
	bgImageGame.setPosition(xPosition, yPosition);
	bgImageGame.setScale(scaleX, 2 * scaleY);

	loseSprite.setTexture(loseImage);
	loseSprite.setScale(gameWidth / loseImage.getSize().x, gameHeight / loseImage.getSize().y);
	loseSprite.setPosition(0, 0);

	return true;

}

void Game::play()
{
	//create a background for the game;
	createImage("../pictures/road_0.png", 0, -gameHeight);
	score = 0;
	//create aleatory street that will hold the obstacle

	//add a sounds for the game
	bool begining = true;//check the begining of the game 
	bool played = false;//check the driving music played or not
	int decrementor = 4;
	sf::Sound CrashSound;
	sf::Sound OpenCar;
	sf::Sound shiftMusic;
	sf::SoundBuffer Crash;
	sf::SoundBuffer shift;
	sf::SoundBuffer Open;
	sf::Music driving;

	Crash.loadFromFile("../music/crash.ogg");
	shift.loadFromFile("../music/shift.wav");
	Open.loadFromFile("../music/open.wav");
	driving.openFromFile("../music/driving.ogg");
	driving.setLoop(true);

	CrashSound.setBuffer(Crash);
	shiftMusic.setBuffer(shift);
	OpenCar.setBuffer(Open);

	std::string selectedPath;
	startInterface(selectedPath);
	OpenCar.play();
	Car car(selectedPath, gameWidth, gameHeight, streetWidth);
	Obstacles obstacles(*this);
	sf::Sprite* carSprite = car.drive();

	window->setFramerateLimit(1000);
	window->clear();

	while (window->isOpen()) {
		sf::Event event;
		if (!played && decrementor < 0)
		{
			driving.setPlayingOffset(sf::milliseconds(1500));
			driving.play();
			played = true;
		}

		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Right:
					car.moveRigt();
					shiftMusic.play();
					break;
				case sf::Keyboard::Left:
					car.moveLeft();
					shiftMusic.play();
					break;
				}
				break;
			}
		}
	if (obstacles.checkLose(carSprite))
	{	
		window->clear();
		window->draw(bgImageGame);
		obstacles.drawObstacles();
		window->draw(*carSprite);
		CrashSound.play();
		driving.stop();
		played = false;


		lose(obstacles);

	}

	window->clear();
	window->draw(bgImageGame);
	obstacles.drawObstacles();
	window->draw(*carSprite);
	moveCar();
	if(decrementor >= 0)
		DecrementerInterface(decrementor--);
	obstacles.moveObstacles();
	drawScore();
	makeItHarder(obstacles);//add more obstacles if the score is high
	window->display();
	}
}

void Game::moveCar()
{
	if (score < 15)
		scrolledYPosition += 0.7f;
	else if (score < 40)
		scrolledYPosition += 1.0f;
	else if (score < 70)
		scrolledYPosition += 1.3f;
	else if (score < 100)
		scrolledYPosition += 1.6f;
	else if (score < 130)
		scrolledYPosition += 1.9f;
	else
		scrolledYPosition += 2.2f;

	if (scrolledYPosition >= 0)
		scrolledYPosition = -gameHeight;

	bgImageGame.setPosition(0, scrolledYPosition);
}

void Game::lose(Obstacles& obstacles) {
	sf::Texture losePic;
	
	std::string scoreMessage = "SCORE    " + std::to_string(score * 10);
	sf::Font font;
	sf::Text Restart;
	sf::Text Home;
	sf::Text exit;

	font.loadFromFile("../fonts/font.ttf");
	scoreText->setString(scoreMessage);
	scoreText->setFont(font);

	scoreText->setScale(1, 1);
	scoreText->setPosition(220, gameHeight - 520);
	scoreText->setFillColor(sf::Color{ 55, 135, 255 });
	Home.setFont(font);
	Home.setCharacterSize(45);
	Home.setString("Home");
	Home.setFillColor(sf::Color{ 4, 117, 7 });
	Home.setPosition(150, 550);
	Restart.setFont(font);
	Restart.setCharacterSize(45);
	Restart.setString("Restart");
	Restart.setFillColor(sf::Color{ 6, 114, 153 });
	Restart.setPosition(400, 550);
	exit.setFont(font);
	exit.setCharacterSize(45);
	exit.setString("Exit");
	exit.setFillColor(sf::Color{ 217, 4, 26 });
	exit.setPosition(275, 620);
	window->draw(loseSprite);
	window->draw(*scoreText);
	window->draw(Home);
	window->draw(exit);
	window->draw(Restart);
	window->display();
	
	
	bool lose = true;

	while (lose) {
		sf::Event event2;
		while (window->pollEvent(event2)) {
			switch (event2.type) {
			case sf::Event::Closed:
				window->close();
				return;
			case sf::Event::MouseButtonPressed:
				if (event2.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2f mousePos(event2.mouseButton.x, event2.mouseButton.y);
					if (Home.getGlobalBounds().contains(mousePos)) {
						play();
						lose = false;
					}
					else if (exit.getGlobalBounds().contains(mousePos)) {
						window->close();
						return;
					}
					else if (Restart.getGlobalBounds().contains(mousePos)) {
						lose = false;
						score = 0;
						obstacles.updateObstacles();//update the obstacles for a new game
					}
				}
				break;
			}
			
		}
	}
}

void Game::makeItHarder(Obstacles& obstacles) {
	if (obstacles.getSize() == 1 && score > 4)
		obstacles.createNewObstacle();
	else if (obstacles.getSize() == 2 && score > 6)
		obstacles.createNewObstacle();
	else if (obstacles.getSize() == 3 && score > 9)
		obstacles.createNewObstacle();

}

void Game::startInterface(std::string& selectedPath) {

	sf::Music startMusic;
	selectedPath = "../car/red.png";
	sf::Texture interface;
	sf::Sprite spriteInterface;
	sf::Text start;
	sf::Text option;	
	sf::Text exit;
	sf::Font font;
	float yPos = -gameHeight;

	interface.loadFromFile("../pictures/road_0.png");
	font.loadFromFile("../fonts/font.ttf");
	start.setCharacterSize(80);
	start.setPosition(200, 200);
	start.setFillColor(sf::Color::Red);
	start.setFont(font);
	start.setString("START");
	option.setCharacterSize(80);
	option.setPosition(60, 300);
	option.setFillColor(sf::Color::Red);
	option.setFont(font);
	option.setString("CHOOSE CAR");
	exit.setCharacterSize(80);
	exit.setPosition(240, 400);
	exit.setFillColor(sf::Color::Red);
	exit.setFont(font);
	exit.setString("EXIT");
	
	spriteInterface.setTexture(interface);
	spriteInterface.setPosition(0, yPos);
	spriteInterface.setScale((gameWidth / (interface.getSize().x)), (gameHeight / (interface.getSize().y))*2);

	sf::Event event;

	//starting the music
	startMusic.openFromFile("../music/start.ogg");
	startMusic.play();

	window->clear();

	while (window->isOpen()) {
		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (start.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
						return ;
					else if (option.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
						chooseCar(selectedPath);
					else if (exit.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						window->close();
						return ;
					}
				}
				break;
			}
		}

		window->draw(spriteInterface);
		window->draw(start);
		window->draw(option);
		window->draw(exit);
		window->display();
		if (yPos >= 0)
			yPos = -gameHeight;
		else
			yPos += 0.1;

		spriteInterface.setPosition(0, yPos);
	}

	window->clear();
	std::cout << "the function has been terminated";

}

void Game::drawScore() {
	sf::Font font;

	font.loadFromFile("../fonts/font.ttf");
	scoreText->setFont(font);
	scoreText->setCharacterSize(30);
	scoreText->setFillColor(sf::Color{ 55, 135, 255 });
	scoreText->setString("Score");
	scoreText->setPosition(10, 10);
	window->draw(*scoreText);
	scoreText->setString(std::to_string(score * 10));
	scoreText->setPosition(15, 40);
	window->draw(*scoreText);
	
}

void Game::chooseCar(std::string& selectedPath) {
	sf::Text home;
	sf::SoundBuffer selectedBuffer;
	sf::Sound selected;
	sf::Text selectedCar;
	sf::Font font;
	sf::Texture interface;
	sf::Sprite spriteInterface;
	float yPos = -gameHeight;
	sf::Event event;

	Car red("../car/red.png", gameWidth, gameHeight, streetWidth);
	Car yellow("../car/yellow.png", gameWidth, gameHeight, streetWidth);
	Car white("../car/white.png", gameWidth, gameHeight, streetWidth);
	Car orange("../car/orange.png", gameWidth, gameHeight, streetWidth);
	Car blue("../car/blue.png", gameWidth, gameHeight, streetWidth);
	Car green("../car/green.png", gameWidth, gameHeight, streetWidth);

	interface.loadFromFile("../pictures/road_0.png");
	spriteInterface.setTexture(interface);
	spriteInterface.setScale((gameWidth / (interface.getSize().x)), (gameHeight / (interface.getSize().y)) * 2);
	spriteInterface.setPosition(0, yPos);


	font.loadFromFile("../fonts/font.ttf");
	home.setCharacterSize(50);
	home.setPosition(30, gameHeight - 100);
	home.setFillColor(sf::Color::Red);
	home.setFont(font);
	home.setString("GO BACK");
	selectedCar.setFont(font);
	selectedCar.setCharacterSize(30);
	selectedCar.setPosition(170, 500);
	selectedCar.setFillColor(sf::Color{ 158, 101, 27 });

	red.drive()->setPosition(100,100);
	yellow.drive()->setPosition(270, 100);
	white.drive()->setPosition(440, 100);
	orange.drive()->setPosition(100, 300);
	green.drive()->setPosition(270, 300);
	blue.drive()->setPosition(440, 300);

	selectedBuffer.loadFromFile("../music/selected.wav");
	selected.setBuffer(selectedBuffer);

	while (window->isOpen()) {
		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::MouseButtonPressed:
				sf::Vector2f mousePosition(event.mouseButton.x, event.mouseButton.y);
				if (red.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("RED CAR SELECTED");
					selected.play();
					selectedPath = red.path;
				}
				else if (yellow.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("YELLOW CAR SELECTED");
					selected.play();
					selectedPath = yellow.path;
				}
				else if (white.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("WHITE CAR SELECTED");
					selected.play();
					selectedPath = white.path;
				}
				else if (orange.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("ORANGE CAR SELECTED");
					selected.play();
					selectedPath = orange.path;
				}
				else if (green.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("GREEN CAR SELECTED");
					selected.play();
					selectedPath = green.path;
				}
				else if (blue.carSprite->getGlobalBounds().contains(mousePosition)) {
					selectedCar.setString("BLUE CAR SELECTED");
					selected.play();
					selectedPath = blue.path;
				}

				else if (home.getGlobalBounds().contains(mousePosition))
				{
					return;
				}
				break;
			}
		}

		window->clear();

		window->draw(spriteInterface);
		window->draw(*red.carSprite);
		window->draw(*yellow.carSprite);
		window->draw(selectedCar);
		window->draw(*white.carSprite);
		window->draw(*orange.carSprite);
		window->draw(*blue.carSprite);
		window->draw(*green.carSprite);
		window->draw(home);
		window->display();

		if (yPos >= 0)
			yPos = -gameHeight;
		else
			yPos += 0.1;

		spriteInterface.setPosition(0, yPos);

	}
	window->clear();
}

void Game::DecrementerInterface(int decrementorValue) {
	sf::Text decrementor;
	sf::Font font;

	font.loadFromFile("../fonts/font.ttf");
	decrementor.setFont(font);
	decrementor.setCharacterSize(150);
	decrementor.setPosition(300, 150);
	decrementor.setFillColor(sf::Color{ 186, 19, 58 });

	decrementor.setString(std::to_string(decrementorValue));
	window->draw(decrementor);
	window->display();
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
}
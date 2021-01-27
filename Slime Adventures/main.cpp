#include <SFML/Graphics.hpp>
#include "iostream";
#include "Player.h";
#include "Coin.h";
#include "Ground.h";
#include "Enemy.h";
#include "Level.h";
#include <sstream>;
#include <cmath>;

void main() {
	int newWidth, newHeight;
	const int screenDimensionX = 1200;
	const int screenDimensionY = 800;
	const int globalBlocSizeX = 40;
	const int globalBlocSizeY = 40;

	sf::RenderWindow window(sf::VideoMode(screenDimensionX, screenDimensionY), "Slime Adventures");	
	
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;

	Level actualLevel("res/levelOne.json");
	actualLevel.setScreenDimension(screenDimensionX, screenDimensionY);
	actualLevel.setGlobalBlocSize(globalBlocSizeX, globalBlocSizeY);
	actualLevel.generateLevel();

	// actualLevel->generateLevel();

	// TEMP
	std::vector<Ground*> collisionTab = actualLevel.getCollisionTab();
	//std::vector<Ground*> grassVector = actualLevel->grassVector;
	// **

	//VIEW
	sf::View view;

	view.reset(sf::FloatRect(0, 0, screenDimensionX, screenDimensionY));
	view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	sf::Vector2f screenPosition(screenDimensionX / 2, screenDimensionY / 2);	

	/*Ground dirt({ globalBlocSizeX,globalBlocSizeY }, &dirtTexture);
	collisionTab.push_back(&dirt);
	dirt.setPos({ 150,500 });
	Ground dirt2({ globalBlocSizeX,globalBlocSizeY }, &dirtTexture);
	collisionTab.push_back(&dirt2);
	dirt2.setPos({ 150,540 });*/

	//Player instance
	sf::Texture playertexture;

	if (!playertexture.loadFromFile("res/img/slime.jpg")) {
		std::cout << "Load failed" << std::endl;

		system("pause");
	}
	Player player({ globalBlocSizeX,globalBlocSizeY }, &playertexture);
	player.setPos({ 50,500 });
	
	//Enemy instance
	sf::Texture enemyTexture;

	if (!enemyTexture.loadFromFile("res/img/slime.jpg")) {
		std::cout << "Load failed" << std::endl;

		system("pause");
	}

	std::vector<Enemy*> enemyVector;
	Enemy enemy1({ globalBlocSizeX, globalBlocSizeY }, &enemyTexture);
	Enemy enemy2({ globalBlocSizeX, globalBlocSizeY }, &enemyTexture);
	enemyVector.push_back(&enemy1);
	enemyVector.push_back(&enemy2);
	enemy1.setPos({ 300, 600 });
	enemy2.setPos({ 500, 600 });

	//Coin instance
	sf::Texture cointexture;

	if (!cointexture.loadFromFile("res/img/coin.png")) {
		std::cout << "Load failed" << std::endl;

		system("pause");
	}
	std::vector<Coin*> coinVector;
	Coin coin1({ 30, 30 }, &cointexture);
	Coin coin2({ 30, 30 }, &cointexture);
	coinVector.push_back(&coin1);
	coinVector.push_back(&coin2);
	coin1.setPos({ 200, 500 });
	coin2.setPos({ 100, 500 });


	//Game Over object
	sf::Font minecraft;
	minecraft.loadFromFile("res/fonts/Minecraft.ttf");

	std::ostringstream ssBigMessage;
	ssBigMessage << "";

	sf::Text lblBigMessage;
	lblBigMessage.setCharacterSize(100);
	lblBigMessage.setPosition({ 300,200 });
	lblBigMessage.setFont(minecraft);
	lblBigMessage.setString(ssBigMessage.str());

	//Score object
	int score = 0;

	std::ostringstream ssScore;
	ssScore << "Score: " << score;

	sf::Text lblScore;
	lblScore.setCharacterSize(30);
	lblScore.setPosition({ 10,10 });
	lblScore.setFont(minecraft);
	lblScore.setString(ssScore.str());

	//Gravity var

	const int groundHeight = 840;
	const float gravitySpeed = 3;

	bool isJumping = false;
	bool isInAir = false;
	bool startClock = false;

	//Main loop
	while (window.isOpen()) {

		sf::Event event;

		//Movement Handling
		const float moveSpeed = 5;
		//Collision
		for (int i = 0; i < collisionTab.size(); i++) {
			if (player.isCollidingWithGround(collisionTab[i])) {
				if (player.getY() < collisionTab[i]->getY()) {
					player.move({ 0.0, -5 });
					isInAir = false;
				}
				else if (player.getY() > collisionTab[i]->getY()) {
					player.move({ 0.0, 5 });
					isInAir = false;
				}
				if (player.getX() < collisionTab[i]->getX() && player.getY() > collisionTab[i]->getY()) {
					player.move({ -5, 0.00 });
				}
				else if (player.getX() > collisionTab[i]->getX() && player.getY() > collisionTab[i]->getY()) {
					player.move({ 5, 0.00 });
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){

			if(isInAir == false){
				player.move({ 0, -moveSpeed - 1.5f });
				isJumping = true;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				player.move({ moveSpeed, 0 });
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				player.move({ -moveSpeed, 0 });
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			player.move({ moveSpeed, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			player.move({ -moveSpeed, 0 });
		}


		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				newWidth = event.size.width;
				newHeight = event.size.height;
				std::cout << "new width: " << event.size.width << std::endl;
				std::cout << "new height: " << event.size.height << std::endl;
				break;
			case sf::Event::KeyReleased:
				isJumping = false;
				if (event.key.code == sf::Keyboard::R) {
					clock.restart();
				}
				break;

			}
		}

		sf::Time elapsed = clock.getElapsedTime();
		//std::cout << elapsed.asSeconds() << std::endl;

		//Gravity logic

		if (player.getY() < groundHeight && isJumping == false) {
			player.move({ 0, gravitySpeed + 2 });
			isInAir = true;

		}


		//Coin logic
		for (int i = 0; i < coinVector.size(); i++) {
			if (player.isCollidingWithCoin(coinVector[i])) {
				coinVector[i]->setPos({ 999999,999999 });
				score++;
				ssScore.str("");
				ssScore << "Score: " << score;
				lblScore.setString(ssScore.str());
			}
		}

		//Enemy logic
		for (int i = 0; i < enemyVector.size(); i++) {
			if (player.isCollidingWithEnemy(enemyVector[i])) {
				//Enemy get killed
				if (player.getY() < enemyVector[i]->getY()) {
					score = score + 10;
					ssScore.str("");
					ssScore << "Score: " << score;
					lblScore.setString(ssScore.str());
					enemyVector[i]->setPos({ 999999,999999 });
				}
				//Player get killed
				else if (player.getY() >= enemyVector[i]->getY()) {
					player.setPos({ 99999,999999 });
					ssBigMessage.str("");
					ssBigMessage << "GAME OVER";
					lblBigMessage.setString(ssBigMessage.str());
				}
			}
		}

		//Player fall off
		if(player.getY() > groundHeight - globalBlocSizeY){
			player.setPos({ 99999,999999 });
			ssBigMessage.str("");
			ssBigMessage << "GAME OVER";
			lblBigMessage.setString(ssBigMessage.str());
		}

		window.clear(); 

		if (player.getX() + 10 > screenDimensionX / 2)
			screenPosition.x = player.getX() + 10;
		else
			screenPosition.x = screenDimensionX / 2;

		view.setCenter(screenPosition);

		// Draw background
		window.draw(actualLevel.getBackground());
		window.draw(lblScore);
		window.draw(lblBigMessage);

		window.setView(view);

		// Maping, drawing.
		/*
		 ** TIPS : Plus un élément est appelé tôt, plus il sera en "arrière plan" par rapport à un autre
		 */

		// Draw playground
		coin1.drawTo(window);
		coin2.drawTo(window);
		enemy1.drawTo(window);
		enemy2.drawTo(window);
		player.drawTo(window);

		actualLevel.drawLevel(window);

		//dirt.drawTo(window); //FOR TEST ONLY
		//dirt2.drawTo(window); //FOR TEST ONLY

		window.setView(window.getDefaultView());

		window.display();
	}
}
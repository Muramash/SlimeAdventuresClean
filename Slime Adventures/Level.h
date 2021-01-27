#ifndef LEVEL_H
#define LEVEL_H
#pragma once

#include <SFML/Graphics.hpp>;
#include <nlohmann/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>;
#include <string>
#include <fstream>
#include <sstream>

#include "Ground.h";

using namespace std;
using json = nlohmann::json;
namespace pt = boost::property_tree;

class Level
{
private:
	// Level file in json
	json levelFile;

	// Screen
	int screenDimensionX;
	int screenDimensionY;

	float globalBlocSizeX;
	float globalBlocSizeY;

	// CollisionTab
	vector<Ground*> collisionTab;

	// Background
	sf::Texture backgroundTexture;
	sf::Sprite background;

	// Ground
	map<int, sf::Texture> allTextures;
	map<int, sf::Texture>::size_type i;
	typedef pair<int, sf::Texture> Int_Texture;

public:
	Level(string levelPath);
	virtual ~Level() = default;

	int generateLevel();

	void drawLevel(sf::RenderWindow& window);

	sf::Sprite getBackground();

	vector<Ground*> getCollisionTab();

	void setScreenDimension(int screenDimensionX, int screenDimensionY);

	void setGlobalBlocSize(int screenDimensionX, int screenDimensionY);
};

#endif // !LEVEL_H
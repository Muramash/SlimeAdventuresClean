#include "Level.h"

Level::Level(string levelPath) {
	// Load the json file in this ptree
	std::ifstream in(levelPath);
	levelFile = json::parse(in);

	for (auto& td : levelFile["groundTextures"])
	{
		string actualTexturePathFile = td["pathFile"];
		int idTexture = td["id"];

		sf::Texture actualTexture;
		if (!actualTexture.loadFromFile(actualTexturePathFile)) {
			cout << "Load failed" << endl;

			system("pause");
		}

		allTextures.insert(Int_Texture(idTexture, actualTexture));
	}

	// Read and save the backgroundTexturePath in *backgroundTexturePath*
	string backgroundTexturePath = levelFile["backgroundTexturePath"];
	if (!backgroundTexture.loadFromFile(backgroundTexturePath)) {
		cout << "Load failed" << endl;

		system("pause");
	}

	screenDimensionX = 0;
	screenDimensionY = 0;

	globalBlocSizeX = 0.F;
	globalBlocSizeY = 0.F;
}

int Level::generateLevel()
{
	background.setTexture(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, screenDimensionX, screenDimensionY));

	float layerX = 0;
	float layerY = screenDimensionY;
	// For each line
	for (auto& td : levelFile["levelDesign"])
	{
		int actualLine = td["line"];
		float actualLayerX = layerX;
		float actualLayerY = layerY - (40 * actualLine);

		// For each call blocks
		for (auto& block : td["blocks"])
		{
			int number = block["number"];
			int id = block["id"];

			if (id != 0)
			{
				// If true block iterate
				for (int i = 0; i < number; i++) {
					Ground* actualGround = new Ground({ globalBlocSizeX, globalBlocSizeY }, &allTextures[id]);
					actualGround->setPos({ actualLayerX, actualLayerY });

					collisionTab.push_back(actualGround);

					actualLayerX = actualLayerX + 40;
				}
			}
			else
			{
				// If void don't use sprite
				actualLayerX = actualLayerX + (40 * number);
			}
		}
	}

	return 1;
}

void Level::drawLevel(sf::RenderWindow& window)
{
	// Get all ground and draw it
	for (int i = 0; i < collisionTab.size(); i++) {
		collisionTab[i]->drawTo(window);
	}
}

sf::Sprite Level::getBackground()
{
	return background;
}

vector<Ground*> Level::getCollisionTab()
{
	return collisionTab;
}

void Level::setScreenDimension(int newScreenDimensionX, int newScreenDimensionY)
{
	screenDimensionX = newScreenDimensionX;
	screenDimensionY = newScreenDimensionY;
}

void Level::setGlobalBlocSize(int newScreenDimensionX, int newScreenDimensionY)
{
	globalBlocSizeX = newScreenDimensionX;
	globalBlocSizeY = newScreenDimensionY;
}
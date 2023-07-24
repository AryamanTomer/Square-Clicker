#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class Game
{ 
private:
	
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Font font;

	sf::Text uiText;
	sf::Text heal;

	bool endGame;
	unsigned int points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;


	vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;


	void initVariables();
	void initWindow();
	void initFont();
	void initText();
	void initEnemies();
public:
	Game();
	virtual ~Game();
	const bool running() const;
	const bool getEndGame() const;
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();
	
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};

#endif
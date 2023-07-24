#include <iostream>
#include <SFML/Audio.hpp>
#include "Game.h"
void Game::initVariables()
{
	this->window = nullptr;

	int points;
	float enemySpawnTimer;
	float enemySpawmTimerMax;
	int maxEnemies;
	this->endGame = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->mouseHeld = false;
}
void Game::initWindow()
{
	this->videomode.height = 600;
	this->videomode.width = 800;
	this->window = new sf::RenderWindow(this->videomode, "Square Clicker!", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}
void Game::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Bold.ttf"))
	{
		cout << "ERROR::GAME::INITFONTS::Failed to load font!" << endl;
	}
}
void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("None");
	this->uiText.setPosition(sf::Vector2f(5.f,5.f));


	this->heal.setFont(this->font);
	this->heal.setCharacterSize(24);
	this->heal.setFillColor(sf::Color::White);
	this->heal.setString("None");
	this->heal.setPosition(sf::Vector2f(695.f, 5.f));

}
void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::Green);
	this->enemy.setOutlineThickness(1.f);
}
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initFont();
	this->initText();
	this->initEnemies();
}
Game::~Game() {
	delete this->window;
}
const bool Game::running() const
{
	return this->window->isOpen();
}
const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setFillColor(sf::Color::Red);
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		break;
	case 1:
		this->enemy.setFillColor(sf::Color::Blue);
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		break;
	case 2:
		this->enemy.setFillColor(sf::Color::Green);
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		break;
	case 3:
		this->enemy.setFillColor(sf::Color::Yellow);
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		break;
	case 4:
		this->enemy.setFillColor(sf::Color::Magenta);
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		break;
	default:
		this->enemy.setFillColor(sf::Color::Black);
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		break;
	}


	this->enemies.push_back(this->enemy);


}

void Game::pollEvents() {
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}
void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::updateText()
{
	stringstream ss;
	ss << "Points: " << this->points;
	this->uiText.setString(ss.str());

	stringstream he;
	he << "Health: " << this->health;
	this->heal.setString(he.str());
}
void Game::updateEnemies()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;
		this->enemies[i].move(0.f, 3.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1.f;
			cout << "Health: " << this->health << endl;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{

				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{

					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
					{
						this->points += 10.f;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow) {
						this->points += 7.f;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Green) {
						this->points += 3.f;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
						this->points += 5.f;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Red) {
						this->points += 1.f;
					}
					this->points += 1.;
					cout << "Points: " << this->points << endl;

					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else {
		this->mouseHeld = false;
	}
	if (this->health <= 0) {
		endGame = true;
	}
	if (endGame == true) {
		this->window->close();
	}
}
void Game::update() {
	this->pollEvents();

	if (!this->endGame)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	if (this->health <= 0)
		this->endGame = true;
	//cout << "Mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << endl;
}
void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
	target.draw(this->heal);
}
void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
void Game::render() {
	this->window->clear(sf::Color(0, 0, 255, 255));
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}
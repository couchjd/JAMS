#include "EntityManager.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>

class Game {
public:
	Game();
	~Game();

	void initialize();
	void drawEverything();
	void run();

private:
	sf::RenderWindow m_main_window;
	sf::CircleShape m_player;
	EntityManager m_entity_manager;

};


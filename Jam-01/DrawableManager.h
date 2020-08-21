#ifndef DRAWABLE_MANAGER_H
#define DRAWABLE_MANAGER_H

#include <SFML/Graphics.hpp>

class DrawableManager {
public:
	DrawableManager();
	~DrawableManager();
	void drawAll();
	void add(const int& id, sf::Drawable* entity);

private:
	sf::Drawable* m_entities[1000];
};

#endif
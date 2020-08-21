#include "DrawableManager.h"



DrawableManager::DrawableManager() {
}


DrawableManager::~DrawableManager() {
}

void DrawableManager::add(const int & id, sf::Drawable* entity) {
	m_entities[id] = entity;
}

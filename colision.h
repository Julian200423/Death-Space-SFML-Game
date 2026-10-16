#pragma once
#include <SFML/Graphics.hpp>
class Collisionable {
	public: //Metodo polimorfico
		virtual sf::FloatRect getBounds() const = 0;
		bool isCollision(Collisionable& col) const;
};

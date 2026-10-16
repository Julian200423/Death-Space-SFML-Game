#pragma once
#include <SFML\Graphics.hpp>
#include "colision.h"

class Personaje : public sf::Drawable, public Collisionable{
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::Vector2f _velocity;
    
public:
    Personaje();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBounds() const override;
    void hited();
};

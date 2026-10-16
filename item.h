#pragma once
#include <SFML/Graphics.hpp>
#include "colision.h"

class Item : public sf::Drawable, public Collisionable {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    
    // Variables para animación
    float _rotation;
    float _floatOffset;
    sf::Vector2f _basePosition; // Posición base sin modificaciones de animación
    
public:
    Item();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void respawn();
    sf::FloatRect getBounds() const override;
};

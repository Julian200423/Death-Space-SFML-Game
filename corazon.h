#pragma once
#include <SFML/Graphics.hpp>
#include "colision.h"

class Corazon : public sf::Drawable, public Collisionable {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    int _timeToDisappear;
    bool _isActive;
    
public:
    Corazon();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void respawn();
    sf::FloatRect getBounds() const override;
    bool isActive() const { return _isActive; }
    void setActive(bool active) { _isActive = active; }
};

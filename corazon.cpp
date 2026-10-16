#include "corazon.h"
#include <stdlib.h>
#include <algorithm>
#include <cmath>

Corazon::Corazon() {
    if (!_texture.loadFromFile("corazon.png")) {
        // Manejar error de carga
    }
    
    _sprite.setTexture(_texture);
    _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height/2);
    _isActive = false;
    _timeToDisappear = 0;
}

void Corazon::update() {
    if (_isActive) {
        // Animación de pulsación para el corazón
        static float pulseTime = 0;
        pulseTime += 0.1f;
        float scale = 1.0f + 0.2f * sin(pulseTime);
        _sprite.setScale(scale, scale);
        
        // Countdown para desaparecer
        _timeToDisappear--;
        if (_timeToDisappear <= 0) {
            _isActive = false;
        }
    }
}

void Corazon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (_isActive) {
        target.draw(_sprite, states);
    }
}

void Corazon::respawn() {
    // Aparecer en posición aleatoria
    float spriteWidth = _sprite.getGlobalBounds().width;
    float spriteHeight = _sprite.getGlobalBounds().height;
    
    float x = std::rand() % int(800 - spriteWidth) + spriteWidth/2;
    float y = std::rand() % int(600 - spriteHeight) + spriteHeight/2;
    
    _sprite.setPosition(x, y);
    _sprite.setScale(1.0f, 1.0f);
    _isActive = true;
    _timeToDisappear = 60 * 10; // 10 segundos antes de desaparecer
}

sf::FloatRect Corazon::getBounds() const {
    if (_isActive) {
        return _sprite.getGlobalBounds();
    }
    return sf::FloatRect();
}


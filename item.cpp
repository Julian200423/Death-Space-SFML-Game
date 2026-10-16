#include <stdlib.h>
#include "item.h"
#include <algorithm>
#include <cmath>

Item::Item() {
    if (!_texture.loadFromFile("moneda.png")) {
        // Manejar error de carga
    }
    
    _sprite.setTexture(_texture);
    _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height/2);
    
    // Inicializar variables para la animación
    _rotation = 0;
    _floatOffset = 0;
    _basePosition = {0, 0};
}

void Item::update() {
    // Animación de rotación
    _rotation += 2.0f;
    if (_rotation >= 360.0f) {
        _rotation = 0.0f;
    }
    _sprite.setRotation(_rotation);
    
    // Efecto de flotación vertical muy sutil
    _floatOffset += 0.05f;
    float floatY = sin(_floatOffset) * 1.0f;
    
    // Aplicar la flotación a la posición base
    _sprite.setPosition(_basePosition.x, _basePosition.y + floatY);
}

void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}

void Item::respawn() {
    // Asegurar que el item aparezca dentro de los límites de pantalla
    float spriteWidth = _sprite.getGlobalBounds().width;
    float spriteHeight = _sprite.getGlobalBounds().height;
    
    float x = std::rand() % int(800 - spriteWidth) + spriteWidth/2;
    float y = std::rand() % int(600 - spriteHeight) + spriteHeight/2;
    
    // Guardar la posición base y establecer la posición del sprite
    _basePosition = {x, y};
    _sprite.setPosition(_basePosition);
    
    // Reiniciar valores de animación
    _rotation = 0;
    _floatOffset = 0;
}

sf::FloatRect Item::getBounds() const {
    return _sprite.getGlobalBounds();
}

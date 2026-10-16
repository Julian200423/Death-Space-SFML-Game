#include "enemigo.h"
#include <stdlib.h>
#include <algorithm>
#include <cmath>

Enemy::Enemy() {
    if (!_texture.loadFromFile("enemigo.png")) {
        // Manejar error de carga
    }
    
    _sprite.setTexture(_texture);
    _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height/2);
    
    if (!_risa.loadFromFile("risa.wav")) {
        // Manejar error de carga de sonido
    }
    _canal.setBuffer(_risa);
    
    _baseSpeed = 1.5f; // Velocidad base del enemigo
    _speed = _baseSpeed;
    _velocity = {0, 0};
    
    respawn();
}

void Enemy::update(sf::Vector2f playerPosition) {
    // Calcular dirección hacia el jugador
    sf::Vector2f enemyPosition = _sprite.getPosition();
    sf::Vector2f direction = playerPosition - enemyPosition;
    
    // Normalizar la dirección
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;
    }
    
    // Aplicar velocidad
    _velocity = direction * _speed;
    _sprite.move(_velocity);
    
    // Cambiar orientación del sprite según la dirección
    if (_velocity.x < 0) {
        _sprite.setScale(-1, 1);
    } else if (_velocity.x > 0) {
        _sprite.setScale(1, 1);
    }
    
    // Mantener dentro de los límites de pantalla
    sf::FloatRect bounds = _sprite.getGlobalBounds();
    sf::Vector2f pos = _sprite.getPosition();
    
    if (bounds.left < 0) {
        _sprite.setPosition(_sprite.getOrigin().x, pos.y);
    }
    if (bounds.top < 0) {
        _sprite.setPosition(pos.x, _sprite.getOrigin().y);
    }
    if (bounds.left + bounds.width > 800) {
        _sprite.setPosition(800 - bounds.width + _sprite.getOrigin().x, pos.y);
    }
    if (bounds.top + bounds.height > 600) {
        _sprite.setPosition(pos.x, 600 - bounds.height + _sprite.getOrigin().y);
    }
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}

void Enemy::respawn() {
    // Aparecer en una esquina aleatoria
    int corner = std::rand() % 4;
    float x, y;
    
    switch(corner) {
        case 0: // Esquina superior izquierda
            x = 50;
            y = 50;
            break;
        case 1: // Esquina superior derecha
            x = 750;
            y = 50;
            break;
        case 2: // Esquina inferior izquierda
            x = 50;
            y = 550;
            break;
        case 3: // Esquina inferior derecha
            x = 750;
            y = 550;
            break;
    }
    
    _sprite.setPosition(x, y);
    _sprite.setScale(1, 1);
}

sf::FloatRect Enemy::getBounds() const {
    return _sprite.getGlobalBounds();
}

void Enemy::youDamaged() {
    respawn();
    _canal.play();
}

void Enemy::increaseDifficulty(float gameTime) {
    // Cada 30 segundos aumenta la velocidad
    float difficultyMultiplier = 1.0f + (gameTime / 30.0f) * 0.3f; // +30% cada 30 segundos
    _speed = std::min(_baseSpeed * difficultyMultiplier, _baseSpeed * 3.0f); // Máximo 3x velocidad base
}


#pragma once
#include <SFML/Graphics.hpp>
#include "colision.h"
#include <SFML/Audio.hpp>

class Enemy : public sf::Drawable, public Collisionable {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::Vector2f _velocity;
    float _speed;
    float _baseSpeed;
    
    // Sonidos
    sf::Sound _canal;
    sf::SoundBuffer _risa;

public:
    Enemy();
    void update(sf::Vector2f playerPosition); // Modificado para recibir posición del jugador
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void respawn();
    void youDamaged();
    sf::FloatRect getBounds() const override;
    void increaseDifficulty(float gameTime); // Basado en tiempo en lugar de puntos
};

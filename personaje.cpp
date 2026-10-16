#include "personaje.h"
#include <SFML/Audio.hpp>
using namespace sf;

Personaje::Personaje(){
    _velocity = {4,4};
    _texture.loadFromFile("nave.png"); // Cambiar a nave.png
    _sprite.setTexture(_texture);
    _sprite.setTextureRect({0,0,64,64}); // Nave estática 64x64
    _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height/2);
    
    // Posición inicial en el centro de la pantalla
    _sprite.setPosition(400, 300);
}

void Personaje::update(){
    _velocity = {0,0};

    // Movimiento simple sin animación
    if(Keyboard::isKeyPressed(Keyboard::Up)){
        _velocity.y = -4;
    }
    if(Keyboard::isKeyPressed(Keyboard::Left)){
        _velocity.x = -4;
    }
    if(Keyboard::isKeyPressed(Keyboard::Down)){
        _velocity.y = 4;
    }
    if(Keyboard::isKeyPressed(Keyboard::Right)){
        _velocity.x = 4;
    }
    
    // No hay animación, la nave es estática
    _sprite.move(_velocity);
    
    // No cambiar escala/orientación, la nave mantiene su orientación
    
    // Evitar que se pase de la pantalla
    if(_sprite.getGlobalBounds().left < 0){
        _sprite.setPosition(_sprite.getOrigin().x, _sprite.getPosition().y);
    }
    
    if(_sprite.getGlobalBounds().top < 0){
        _sprite.setPosition(_sprite.getPosition().x, _sprite.getOrigin().y);
    }
    
    if(_sprite.getGlobalBounds().left + _sprite.getGlobalBounds().width > 800){
        _sprite.setPosition(800 - (_sprite.getGlobalBounds().width - _sprite.getOrigin().x), _sprite.getPosition().y);
    }
    
    if(_sprite.getGlobalBounds().top + _sprite.getGlobalBounds().height > 600){
        _sprite.setPosition(_sprite.getPosition().x, 600 - (_sprite.getGlobalBounds().height - _sprite.getOrigin().y));
    }
}

void Personaje::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}

sf::FloatRect Personaje::getBounds() const{
    return _sprite.getGlobalBounds();
}

void Personaje::hited(){
    // Aquí puedes agregar efectos cuando la nave es golpeada
}

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "personaje.h" 
#include "item.h"
#include "corazon.h"
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "enemigo.h"
using namespace sf;
using namespace std;

int main() {
    try {
        // Inicialización
        srand((unsigned)time(0));
        
        RenderWindow window(VideoMode(800, 600), "Juego de Supervivencia - Nave Espacial");
        window.setFramerateLimit(60);
        
        // Cargar fondo
        Texture fondoTexture;
        Sprite fondoSprite;
        if (fondoTexture.loadFromFile("fondo_espacio.png")) {
            fondoSprite.setTexture(fondoTexture);
            fondoSprite.setScale(800.0f / fondoTexture.getSize().x, 600.0f / fondoTexture.getSize().y);
        }
        
        // Configuración de texto
        Font font;
        bool fontLoaded = font.loadFromFile("Texto.ttf");
        
        Text textPuntos, textVidas, textGameOver, textTiempo, textDificultad, textNivel;
        
        // Configurar textos
        if (fontLoaded) {
            textPuntos.setFont(font);
            textVidas.setFont(font);
            textTiempo.setFont(font);
            textDificultad.setFont(font);
            textGameOver.setFont(font);
            textNivel.setFont(font);
        }
        
        textPuntos.setPosition(10, 10);
        textPuntos.setFillColor(Color::White);
        textPuntos.setCharacterSize(24);
        
        textVidas.setPosition(10, 50);
        textVidas.setFillColor(Color::White);
        textVidas.setCharacterSize(24);
        
        textTiempo.setPosition(10, 90);
        textTiempo.setFillColor(Color::Cyan);
        textTiempo.setCharacterSize(20);
        
        textDificultad.setPosition(10, 120);
        textDificultad.setFillColor(Color::Yellow);
        textDificultad.setCharacterSize(18);
        
        textGameOver.setPosition(150, 200);
        textGameOver.setFillColor(Color::Red);
        textGameOver.setCharacterSize(36);
        
        // Texto de nivel (grande, centrado)
        textNivel.setPosition(400, 300);
        textNivel.setFillColor(Color::White);
        textNivel.setCharacterSize(72);
        textNivel.setStyle(Text::Bold);
        
        // Audio de fondo
        Music musicaFondo;
        if (musicaFondo.openFromFile("fondo.wav")) {
            musicaFondo.setLoop(true);
            musicaFondo.setVolume(20); // 50% de volumen
            musicaFondo.play();
        }
        
        // Sonidos de efectos
        SoundBuffer bufferCorazon, bufferMoneda;
        Sound soundCorazon, soundMoneda;
        
        if (bufferCorazon.loadFromFile("corazon_sound.wav")) {
            soundCorazon.setBuffer(bufferCorazon);
        }
        
        if (bufferMoneda.loadFromFile("moneda_sound.wav")) {
            soundMoneda.setBuffer(bufferMoneda);
        }
        
        // Objetos del juego
        Personaje nave; // Ahora es una nave
        Item item;
        item.respawn();
        
        Enemy enemy;
        Corazon corazon;
        
        // Variables del juego
        int puntos = 0;
        int vidas = 3;
        bool gameOver = false;
        int nivelActual = 1;
        int nivelAnterior = 0;
        bool mostrandoNivel = true;
        
        // Sistemas de tiempo
        Clock gameClock;
        Clock collisionClock;
        Clock heartSpawnClock;
        Clock nivelClock; // Para mostrar el texto de nivel
        const float COLLISION_COOLDOWN = 1.0f;
        const float HEART_SPAWN_INTERVAL = 45.0f;
        const float NIVEL_DISPLAY_TIME = 3.0f; // 3 segundos mostrando el nivel
        
        // Sonidos
        SoundBuffer bufferRisa;
        bufferRisa.loadFromFile("risa.wav");
        Sound soundRisa;
        soundRisa.setBuffer(bufferRisa);
        
        cout << "Juego iniciado correctamente" << endl;
        
        // Bucle principal del juego
        while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Escape)) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
            
            float gameTime = gameClock.getElapsedTime().asSeconds();
            
            // Calcular nivel actual basado en tiempo (cada 30 segundos)
            nivelActual = (int)(gameTime / 30) + 1;
            
            // Detectar cambio de nivel
            if (nivelActual != nivelAnterior && !gameOver) {
                mostrandoNivel = true;
                nivelClock.restart();
                nivelAnterior = nivelActual;
                
                // Centrar el texto de nivel
                ostringstream ssNivelDisplay;
                ssNivelDisplay << "NIVEL " << setfill('0') << setw(2) << nivelActual;
                textNivel.setString(ssNivelDisplay.str());
                
                FloatRect textBounds = textNivel.getLocalBounds();
                textNivel.setOrigin(textBounds.left + textBounds.width/2.0f, 
                                   textBounds.top + textBounds.height/2.0f);
                textNivel.setPosition(400, 300);
            }
            
            // Ocultar texto de nivel después de 3 segundos
            if (mostrandoNivel && nivelClock.getElapsedTime().asSeconds() > NIVEL_DISPLAY_TIME) {
                mostrandoNivel = false;
            }
            
            if (!gameOver && !mostrandoNivel) {
                // Actualizar objetos solo si no estamos mostrando el nivel
                nave.update();
                
                sf::FloatRect bounds = nave.getBounds();
                enemy.update(sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2));
                
                item.update();
                corazon.update();
                
                // Aumentar dificultad basada en tiempo
                enemy.increaseDifficulty(gameTime);
                
                // Spawn de corazones cada 45 segundos (sin probabilidad)
                if (heartSpawnClock.getElapsedTime().asSeconds() > HEART_SPAWN_INTERVAL && !corazon.isActive()) {
                    corazon.respawn();
                    heartSpawnClock.restart();
                }
                
                // Colisión con item (moneda)
                if (nave.isCollision(item)) {
                    item.respawn();
                    puntos++;
                    soundMoneda.play(); // Sonido de moneda
                }
                
                // Colisión con corazón
                if (corazon.isActive() && nave.isCollision(corazon)) {
                    vidas++;
                    vidas = std::min(vidas, 9);
                    corazon.setActive(false);
                    heartSpawnClock.restart();
                    soundCorazon.play(); // Sonido de corazón
                }
                
                // Colisión con enemigo (con cooldown)
                if (nave.isCollision(enemy) && collisionClock.getElapsedTime().asSeconds() > COLLISION_COOLDOWN) {
                    nave.hited();
                    enemy.youDamaged();
                    vidas--;
                    collisionClock.restart();
                    
                    if (vidas <= 0) {
                        gameOver = true;
                        musicaFondo.pause(); // Pausar música de fondo
                    }
                }
            }
            else if (gameOver) {
                // Game Over - reiniciar con SPACE
                if (Keyboard::isKeyPressed(Keyboard::Space)) {
                    vidas = 3;
                    puntos = 0;
                    gameOver = false;
                    nivelActual = 1;
                    nivelAnterior = 0;
                    mostrandoNivel = true;
                    enemy.respawn();
                    item.respawn();
                    corazon.setActive(false);
                    gameClock.restart();
                    collisionClock.restart();
                    heartSpawnClock.restart();
                    nivelClock.restart();
                    musicaFondo.play(); // Reanudar música de fondo
                }
            }
            
            // Actualizar textos
            ostringstream ssPuntos;
            ssPuntos << "Puntos: " << puntos;
            textPuntos.setString(ssPuntos.str());
            
            ostringstream ssVidas;
            ssVidas << "Vidas: " << vidas;
            textVidas.setString(ssVidas.str());
            
            // Mostrar tiempo de supervivencia
            if (!gameOver) {
                ostringstream ssTiempo;
                int minutos = (int)gameTime / 60;
                int segundos = (int)gameTime % 60;
                ssTiempo << "Tiempo: " << setfill('0') << setw(2) << minutos 
                         << ":" << setfill('0') << setw(2) << segundos;
                textTiempo.setString(ssTiempo.str());
            }
            
            // Mostrar nivel de dificultad
            ostringstream ssDificultad;
            ssDificultad << "Nivel: " << setfill('0') << setw(2) << nivelActual;
            textDificultad.setString(ssDificultad.str());
            
            // Game Over text
            if (gameOver) {
                ostringstream ssGameOver;
                int minutosFinal = (int)gameTime / 60;
                int segundosFinal = (int)gameTime % 60;
                ssGameOver << "GAME OVER\n"
                          << "Puntos totales: " << puntos << "\n"
                          << "Nivel alcanzado: " << nivelActual << "\n\n"
                          << "Presiona SPACE para jugar de nuevo";
                textGameOver.setString(ssGameOver.str());
            }
            
            // Renderizado
            window.clear(Color::Black);
            
            // Dibujar fondo
            if (fondoTexture.getSize().x > 0) {
                window.draw(fondoSprite);
            }
            
            if (!gameOver) {
                if (!mostrandoNivel) {
                    // Dibujar objetos del juego (solo si no estamos mostrando nivel)
                    window.draw(nave);
                    window.draw(item);
                    window.draw(enemy);
                    if (corazon.isActive()) {
                        window.draw(corazon);
                    }
                    
                    // Dibujar interfaz durante el juego
                    if (fontLoaded) {
                        window.draw(textPuntos);
                        window.draw(textVidas);
                        window.draw(textTiempo);
                        window.draw(textDificultad);
                    }
                } else {
                    // Mostrar solo el texto de nivel grande
                    if (fontLoaded) {
                        window.draw(textNivel);
                    }
                }
            } else {
                if (fontLoaded) {
                    window.draw(textGameOver);
                }
            }
            
            window.display();
        }
        
        // Detener música al salir
        musicaFondo.stop();
        
    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}

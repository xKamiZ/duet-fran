/*
 * PLAYER CLASS
 * AUTHOR: Fran Caamaño Martínez
 */

#ifndef BASICS_PROJECT_TEMPLATE_PLAYER_HPP
#define BASICS_PROJECT_TEMPLATE_PLAYER_HPP

#include <vector>

#include "Sprite.hpp"

using std::vector;
using std::shared_ptr;
using basics::Point2f;

namespace DuetClone {

    class Player{

        typedef  shared_ptr<Sprite> Sprite_Pointer;
        typedef  vector<Sprite_Pointer> Player_Sprites;

    private:

        float _verticalSpeed;                                                // Velocidad de movimiento vertical del Player
        float _rotationSpeed;                                                // Velocidad de rotación del Player

        Player_Sprites _playerSprites {} ;                                   // Vector de punteros a sprites

        Point2f _rotationPivotPoint;                                         // Punto de pivote para la rotación de los sprites del Player

    public:

        // Constructor por defecto
        Player();

        // Añade un nuevo puntero a sprite al final del array de sprites del jugador
        void AddPlayerSprite(const shared_ptr<Sprite> spriteRef) { _playerSprites.push_back(spriteRef); }
        void RenderPlayer(Canvas & canvas);                                  // Dibuja los sprites del jugador en pantalla
        void MoveRight(float deltaTime);                                     // Rota el jugador hacia la derecha a través del punto de pivote
        void MoveLeft(float deltaTime);                                      // Rota el jugador hacia la izquierda a través del punto de pivote
        bool PlayerCollided(Sprite & other);                                 // Comprueba si alguno de los sprites del jugador a impactado con otro
    };

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_PLAYER_HPP
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

    private:

        static constexpr float _rotationSpeed = 3.0f;                                  // Velocidad de rotación del Player
        vector<shared_ptr<Sprite>> _playerSprites {} ;                                   // Vector de punteros a sprites
        Point2f _rotationPivotPoint;                                         // Punto de pivote para la rotación de los sprites del Player
        float _currentAngle;
        float _direction;

    public:

        // Constructor por defecto
        Player();

        /// SETTERS ///
        void SetPivotPoint(float x, float y) { _rotationPivotPoint[0] = x; _rotationPivotPoint[1] = y;}
        void IncrementCurrentAngle(float newAngle) { _currentAngle = newAngle; }
        void SetDirection(float newDirection) { _direction = newDirection; }

        // Añade un nuevo puntero a sprite al final del array de sprites del jugador
        void AddPlayerSprite(const shared_ptr<Sprite> spriteRef) { _playerSprites.push_back(spriteRef); }
        void RenderPlayer(Canvas & canvas);                                  // Dibuja los sprites del jugador en pantalla
        void UpdatePlayer(float deltaTime, bool touchingScreen);                                  // Actualiza el jugador en Update
        bool PlayerCollided(Sprite & other);                                 // Comprueba si alguno de los sprites del jugador a impactado con otro

    private:

        void RotateCircleSprite(Sprite & targetSprite, float angleWithPivot);

    };

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_PLAYER_HPP

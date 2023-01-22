/*
 * PLAYER CLASS
 * AUTHOR: Fran Caamaño Martínez
 */

#include "Player.hpp"

namespace DuetClone {


    Player::Player()
    {

        _verticalSpeed = 400.0f;
        _rotationSpeed = 50.0f;

    }

    void Player::RenderPlayer(Canvas & canvas)
    {
        // Si no existen sprites en el array, sale de la función
        if (_playerSprites.empty()) return;

        // Loopea el array de sprites y llama a la función render de cada referencia al elemento
        for (const auto& sprite : _playerSprites) sprite->render(canvas);
    }

    void Player::MoveLeft(float deltaTime)
    {

    }

    void Player::MoveRight(float deltaTime)
    {

    }
} // DuetClone
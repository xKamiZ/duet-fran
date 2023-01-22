/*
 * PLAYER CLASS
 * AUTHOR: Fran Caamaño Martínez
 */

#include "Player.hpp"
#include <math.h>

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

    bool Player::PlayerCollided(Sprite & other)
    {
        for (const auto& sprite : _playerSprites)
        {
            if (sprite->intersects(other)) return true;
            else return false;
        }
        return false;
    }

    void Player::UpdatePlayer(float deltaTime)
    {
        // Update de los sprites
        for (const auto& sprite : _playerSprites) sprite->update(deltaTime);

        // Mueve los sprites del player a una velocidad vertical constante
        HandleVerticalPlayerMovement();
    }

    void Player::RotateRight()
    {
        // Si la dirección es negativa, la cambia a positiva
        if (_rotationSpeed < 0.0f) SetRotationSpeed(abs(_rotationSpeed));
    }

    void Player::RotateLeft()
    {
        // Si la dirección es positiva, la cambia a negativa
        if (_rotationSpeed > 0.0f) SetRotationSpeed(-_rotationSpeed);
    }

    void Player::HandleVerticalPlayerMovement()
    {
        for (auto & sprite : _playerSprites) sprite->set_speed_y(_verticalSpeed);
    }
} // DuetClone
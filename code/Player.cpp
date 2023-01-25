/*
 * PLAYER CLASS
 * AUTHOR: Fran Caamaño Martínez
 */

#include "Player.hpp"
#include <cmath>

namespace DuetClone {


    Player::Player()
    {
        _rotationPivotPoint[0] = 0.0f;
        _rotationPivotPoint[1] = 0.0f;
    }

    void Player::RenderPlayer(Canvas & canvas)
    {
        // Si no existen sprites en el array, sale de la función
        if (_playerSprites.empty()) return;

        // Loopea el array de sprites y llama a la función render de cada referencia al elemento
        for (auto& sprite : _playerSprites) sprite->render(canvas);
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

    void Player::UpdatePlayer(float deltaTime, bool touchingScreen)
    {
        // Update de los sprites
        for (const auto & sprite : _playerSprites) sprite->update(deltaTime);

        if (touchingScreen)
        {
            IncrementCurrentAngle(_direction * deltaTime * _rotationSpeed);

            RotateCircleSprite(*_playerSprites[0].get(), _currentAngle);
            RotateCircleSprite(*_playerSprites[1].get(), _currentAngle);
        }
        else _currentAngle = 0.0f;

    }

    void Player::RotateCircleSprite(Sprite & targetSprite, float angleWithPivot)
    {
        float angleSin = sin(angleWithPivot);
        float angleCos = cos(angleWithPivot);

        float spriteXPosition = targetSprite.get_position_x();
        float spriteYPosition = targetSprite.get_position_y();

        spriteXPosition -= _rotationPivotPoint[0];
        spriteYPosition -=  _rotationPivotPoint[1];

        targetSprite.set_position_x(spriteXPosition);
        targetSprite.set_position_y(spriteYPosition);

        float newXSecondSprite = (targetSprite.get_position_x() * angleCos) - (targetSprite.get_position_y() * angleSin);
        float newYSecondSprite = (targetSprite.get_position_x() * angleSin) + (targetSprite.get_position_y() * angleCos);

        spriteXPosition = newXSecondSprite + _rotationPivotPoint[0];
        spriteYPosition = newYSecondSprite + _rotationPivotPoint[1];

        targetSprite.set_position_x(spriteXPosition);
        targetSprite.set_position_y(spriteYPosition);
    }

} // DuetClone
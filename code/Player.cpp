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
            Rotate(_currentAngle);
        }
        else _currentAngle = 0.0f;


    }

    void Player::Rotate(float ball0Angle)
    {
        RotateBall0(ball0Angle);
        RotateBall1(ball0Angle + M_PI);
    }

    void Player::RotateBall0(float angleWithPivot)
    {
        auto ball0Sprite = _playerSprites[0].get();

        float angleSin = sin(angleWithPivot);
        float angleCos = cos(angleWithPivot);

        float ball0xPosition = ball0Sprite->get_position_x();
        float ball0yPosition = ball0Sprite->get_position_y();

        ball0xPosition -= _rotationPivotPoint[0];
        ball0yPosition -=  _rotationPivotPoint[1];

        ball0Sprite->set_position_x(ball0xPosition);
        ball0Sprite->set_position_y(ball0yPosition);

        float newXFirstSprite = (_playerSprites[0]->get_position_x() * angleCos) - (_playerSprites[0]->get_position_y() * angleSin);
        float newYFirstSprite = (_playerSprites[0]->get_position_x() * angleSin) + (_playerSprites[0]->get_position_y() * angleCos);

        ball0xPosition = newXFirstSprite + _rotationPivotPoint[0];
        ball0yPosition = newYFirstSprite + _rotationPivotPoint[1];

        ball0Sprite->set_position_x(ball0xPosition);
        ball0Sprite->set_position_y(ball0yPosition);

    }

    void Player::RotateBall1(float angleWithPivot)
    {
        auto ball1Sprite = _playerSprites[1].get();

        float angleSin = sin(angleWithPivot);
        float angleCos = cos(angleWithPivot);

        float ball1xPosition = ball1Sprite->get_position_x();
        float ball1yPosition = ball1Sprite->get_position_y();

        ball1xPosition -= _rotationPivotPoint[0];
        ball1yPosition -=  _rotationPivotPoint[1];

        ball1Sprite->set_position_x(ball1xPosition);
        ball1Sprite->set_position_y(ball1yPosition);

        float newXSecondSprite = (_playerSprites[1]->get_position_x() * angleCos) - (_playerSprites[1]->get_position_y() * angleSin);
        float newYSecondSprite = (_playerSprites[1]->get_position_x() * angleSin) + (_playerSprites[1]->get_position_y() * angleCos);

        ball1xPosition = newXSecondSprite + _rotationPivotPoint[0];
        ball1yPosition = newYSecondSprite + _rotationPivotPoint[1];

        ball1Sprite->set_position_x(ball1xPosition);
        ball1Sprite->set_position_y(ball1yPosition);
    }

} // DuetClone
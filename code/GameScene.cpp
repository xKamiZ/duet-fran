/*
 * GAME SCENE
 * AUTHOR: Fran Caamaño Martínez
 */

#include "GameScene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>

using namespace basics;
using namespace std;

namespace DuetClone
{

    GameScene::Texture_Data GameScene::_texturesData[] =
            {
                    { ID(blueCircleId),    "high/blue-circle.png"               },
                    { ID(redCircleId),       "high/red-circle.png"              },
            };

    unsigned GameScene::_texturesCount = sizeof(_texturesData) / sizeof(Texture_Data);

    GameScene::GameScene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool GameScene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        return true;
    }

    void GameScene::suspend ()
    {
        suspended = true;
    }

    void GameScene::resume ()
    {
        suspended = false;
    }

    void GameScene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    if (x > canvas_width / 2.0f)
                    {

                    } // Pulsación en la mitad derecha de la pantalla
                    else if (x < canvas_width / 2.0f)
                    {

                    } // Pulsación en la mitad izquierda de la pantalla


                    break;
                }
                case ID(touch-ended):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                    break;
                }
            }
        }
    }

    void GameScene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void GameScene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear();


                switch (state)
                {
                    case LOADING:
                    case RUNNING:

                        canvas->set_color(0, 0.5f, 1);
                        canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height});

                        RenderSprites(*canvas);

                        break;
                }
            }
        }
    }

    void GameScene::load ()
    {
        if (!suspended)
        {
            GameScene::GraphicsContext context = director.lock_graphics_context ();

            if (context) LoadTextures(context);
        }
    }

    void GameScene::run (float deltaTime)
    {
        UpdateSprites(deltaTime);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::LoadTextures(GameScene::GraphicsContext & graphicsContext)
    {
        // Se ejecuta por cada textura del array de texturas
        if (_textures.size() < _texturesCount)
        {
            // Obtiene la información de las texturas del array de texturas
            Texture_Data & currentTextureData = _texturesData[_textures.size ()];
            Texture_Pointer & currentTexture = _textures[currentTextureData.id] = Texture_2D::create (currentTextureData.id, graphicsContext, currentTextureData.path);

            // Añade la textura al contexto gráfico
            if (currentTexture) graphicsContext->add (currentTexture);
        }
        else
        {
            if (_timer.get_elapsed_seconds() > 1.0f)
            {
                CreateSprites();

                state = RUNNING;
            }
        }
    }

    void GameScene::CreateSprites()
    {
        Sprite_Pointer blueCircle(new Sprite(_textures[ID(blueCircleId)].get()));
        Sprite_Pointer redCircle(new Sprite(_textures[ID(redCircleId)].get()));

        //_sprites.push_back(blueCircle);
        //_sprites.push_back(redCircle);

        // Añade al array de sprites de player los sprites con las texturas correspondientes cargadas
        if (_player)
        {
            _player->AddPlayerSprite(blueCircle);
            _player->AddPlayerSprite(redCircle);
        }
    }

    void GameScene::RenderSprites(Canvas & canvas)
    {
        // Dibuja el objeto jugador
        _player->RenderPlayer(canvas);
    }

    void GameScene::UpdateSprites(float deltaTime)
    {
        _player->UpdatePlayer(deltaTime);
    }

}
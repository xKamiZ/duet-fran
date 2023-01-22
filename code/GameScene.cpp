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
        _isAspectRatioAdjusted = false;

        canvas_width  = 1920;
        canvas_height = 1080;

        _playerPtr = &player;
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

                        canvas->set_color(1.0f, 1.0f, 1.0f);
                        canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height});

                        RenderSprites(*canvas);

                        constexpr int square_size    = 150;
                        constexpr int square_spacing = 25;

                        for (int y = square_spacing, row = 0; y < canvas_height; y += square_size + square_spacing, row++)
                        {
                            bool fill = (row & 1) == 0;

                            for (int x = square_spacing; x < canvas_width; x += square_size + square_spacing)
                            {
                                if (fill)
                                {
                                    canvas->fill_rectangle ({ x, y }, { square_size, square_size });
                                    fill = false;
                                }
                                else
                                {
                                    canvas->draw_rectangle ({ x, y }, { square_size, square_size });
                                    fill = true;
                                }
                            }
                        }

                        break;
                }
            }
        }
    }

    void GameScene::load ()
    {
        if (!suspended)
        {
            GameScene::GraphicsContextAccessor context = director.lock_graphics_context ();

            if (context)
            {
                // Ajusta el aspect ratio si no lo está
                if (!_isAspectRatioAdjusted) AdjustAspectRatio(context);

                LoadTextures(context);
            }
        }
    }

    void GameScene::run (float deltaTime)
    {
        UpdateSceneObjects(deltaTime);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::AdjustAspectRatio(GameScene::GraphicsContextAccessor & context)
    {
        /*
         * Teniendo en cuenta que la orientación de la pantalla del juego será siempre
         * 'portrait', el ancho será menor que el alto.
         * El alto será reescalado para que coincida con el aspect ratio real de la ventana.
         */

        float aspectRatio = float(context->get_surface_width()) / context->get_surface_height();

        canvas_height = unsigned(canvas_width * aspectRatio);

        _isAspectRatioAdjusted = true;
    }

    void GameScene::LoadTextures(GameScene::GraphicsContextAccessor & graphicsContext)
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
        if (_playerPtr)
        {
            _playerPtr->AddPlayerSprite(blueCircle);
            _playerPtr->AddPlayerSprite(redCircle);
        }
    }

    void GameScene::RenderSprites(Canvas & canvas)
    {
        // Dibuja el objeto jugador
        if (_playerPtr) _playerPtr->RenderPlayer(canvas);
    }

    void GameScene::UpdateSceneObjects(float deltaTime)
    {
        // Llama al update del player
        if (_playerPtr) _playerPtr->UpdatePlayer(deltaTime);
    }

}
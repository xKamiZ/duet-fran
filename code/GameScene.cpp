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
                    { ID(blueCircleId),    "high/blue-circle.png"             },
                    { ID(redCircleId),     "high/red-circle.png"              },
                    { ID(rect01Id),        "high/rectangle-01.png"            },
                    { ID(rect02Id),        "high/rectangle-02.png"            },
                    { ID(rect03Id),        "high/rectangle-03.png"            },
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

                        canvas->set_color(0.0f, 0.0f, 0.0f);
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
            GameScene::GraphicsContextAccessor context = director.lock_graphics_context ();

            if (context)
            {
                // Ajusta el aspect ratio si no lo está
                if (!_isAspectRatioAdjusted) AdjustAspectRatio(context);

                LoadTextures(context);

                InitSceneObjects();
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

        canvas_width = unsigned(canvas_height * aspectRatio);

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
        Sprite_Pointer rectangle01(new Sprite(_textures[ID(rect01Id)].get()));
        Sprite_Pointer rectangle02(new Sprite(_textures[ID(rect02Id)].get()));
        Sprite_Pointer rectangle03(new Sprite(_textures[ID(rect03Id)].get()));

        // Inicializa los puntos centrales de los sprites de los obstáculos
        rectangle01->set_anchor(basics::CENTER);
        rectangle02->set_anchor(basics::CENTER);
        rectangle03->set_anchor(basics::CENTER);

        // Añade a la lista los obstáculos los tres sprites de obstáculos
        _obstacleSprites.push_back(rectangle01);
        _obstacleSprites.push_back(rectangle02);
        _obstacleSprites.push_back(rectangle03);

        // Posiciona los sprites de ambos círculos en la pantalla
        if (blueCircle) blueCircle->set_position_x(canvas_width / 4.0f);
        if (blueCircle) blueCircle->set_position_y(canvas_height / 4.0f);

        if (redCircle) redCircle->set_position_x(canvas_width * 0.75f);
        if (redCircle) redCircle->set_position_y(canvas_height / 4.0f);

        // Añade al array de sprites de player los sprites con las texturas correspondientes cargadas
        if (_playerPtr)
        {
            _playerPtr->AddPlayerSprite(blueCircle);
            _playerPtr->AddPlayerSprite(redCircle);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::InitSceneObjects()
    {
        // Establece el punto de pivote de rotación
        if(_playerPtr) _playerPtr->SetPivotPoint(canvas_width / 2.0f, canvas_height / 8.0f);

        // Establece la posición inicial de los obstáculos
        for (auto & obstacleSprite : _obstacleSprites)
        {
            obstacleSprite->set_position_x(canvas_width / 2.0f);
            obstacleSprite->set_position_y(canvas_height);
        }

        // Establece la velocidad inicial en vertical de los obstáculos
        for (auto & obstacleSprite : _obstacleSprites) obstacleSprite->set_speed_y(0.0f);
    }

    void GameScene::RenderSprites(Canvas & canvas)
    {
        // Dibuja el objeto jugador
        if (_playerPtr) _playerPtr->RenderPlayer(canvas);

        // Dibuja todos los obstáculos de la lista
        for (const auto & obstacleSprite : _obstacleSprites) obstacleSprite->render(canvas);
    }

    void GameScene::UpdateSceneObjects(float deltaTime)
    {
        // Llama a update en el player
        if (_playerPtr) _playerPtr->UpdatePlayer(deltaTime);

        // Llama a update para cada obstáculo
        for (auto & obstacleSprite : _obstacleSprites)
        {
            obstacleSprite->set_speed_y(_obstaclesDefaultVerticalSpeed);
            obstacleSprite->update(deltaTime);
        }
    }

}
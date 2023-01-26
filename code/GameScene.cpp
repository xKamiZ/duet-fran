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
                    { ID(pauseButtonId),   "high/ui/pause-button.png"         },
            };

    unsigned GameScene::_texturesCount = sizeof(_texturesData) / sizeof(Texture_Data);

    GameScene::GameScene()
    {
        canvas_width  = 1920;
        canvas_height = 1080;

        _isAspectRatioAdjusted = false;
        _pauseButton = nullptr;
        _obstacleList = { };
        _obstaclePool = { };
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
        if (state == RUNNING || state == PAUSED)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    _touchingScreen = true;

                    if (x > canvas_width / 2.0f)  _player.SetDirection(-1.0f);            // Pulsa la mitad derecha de la pantalla
                    else if (x < canvas_width / 2.0f) _player.SetDirection(1.0f);         // Pulsa la mitad izquierda de la pantalla

                    break;
                }
                case ID(touch-ended):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    _touchingScreen = false;

                    Point2f touchPosition = {x, y};

                    CheckForPause(touchPosition);                                                    // Comprueba si se ha pulsado el botón de pausa

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
            case PAUSED: break;
        }
    }

    void GameScene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            if (state == RUNNING)
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
                        case PAUSED:

                            canvas->set_color(0.0f, 0.0f, 0.0f);
                            canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height});

                            RenderPauseMenu(*canvas);

                            break;
                    }
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
            std::shared_ptr<Texture_2D> & currentTexture = _textures[currentTextureData.id] = Texture_2D::create (currentTextureData.id, graphicsContext, currentTextureData.path);

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
        // Crea los Sprites que aparecerán en la escena
        std::shared_ptr<Sprite> blueCircle(new Sprite(_textures[ID(blueCircleId)].get()));
        std::shared_ptr<Sprite> redCircle(new Sprite(_textures[ID(redCircleId)].get()));
        std::shared_ptr<Sprite> rectangle01(new Sprite(_textures[ID(rect01Id)].get()));
        std::shared_ptr<Sprite> rectangle02(new Sprite(_textures[ID(rect02Id)].get()));
        std::shared_ptr<Sprite> rectangle03(new Sprite(_textures[ID(rect03Id)].get()));

        // Crea el Sprite del botón de pausa y lo guarda en el puntero
        _pauseButton.reset (new Sprite(_textures[ID(pauseButtonId)].get()));

        // Obtiene el punto donde se posicionará el Sprite
        const float heightOffset = 70.0f;
        const float widthOffset = 50.0f;
        std::shared_ptr<Point2f> pauseButtonPosition (new Point2f(canvas_width - widthOffset, canvas_height - heightOffset));

        // Establece el punto de anclaje y posición del Sprite en la pantalla
        _pauseButton->set_anchor(CENTER);
        _pauseButton->set_position(*pauseButtonPosition.get());

        // Inicializa los puntos centrales de los sprites de los obstáculos
        rectangle01->set_anchor(basics::CENTER);
        rectangle02->set_anchor(basics::TOP | basics::RIGHT);
        rectangle03->set_anchor(basics::TOP | basics::LEFT);

        // Posiciona los sprites de ambos círculos en la pantalla
        if (blueCircle) blueCircle->set_position_x(canvas_width / 4.0f);
        if (blueCircle) blueCircle->set_position_y(canvas_height / 4.0f);
        if (redCircle) redCircle->set_position_x(canvas_width * 0.75f);
        if (redCircle) redCircle->set_position_y(canvas_height / 4.0f);

        // Añade los Sprites de los obstáculos al pool
        _obstaclePool.Add(rectangle01.get());
        _obstaclePool.Add(rectangle02.get());
        _obstaclePool.Add(rectangle03.get());


        _spriteList.push_back(blueCircle);
        _spriteList.push_back(redCircle);
        _spriteList.push_back(rectangle01);
        _spriteList.push_back(rectangle02);
        _spriteList.push_back(rectangle03);


        // Añade al array de sprites de _player los sprites de las bolas
        _player.AddPlayerSprite(blueCircle);
        _player.AddPlayerSprite(redCircle);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::InitSceneObjects()
    {
        // Establece el punto de pivote de rotación
        _player.SetPivotPoint(canvas_width / 2.0f, canvas_height / 6.0f);

        for (int i = 0; i < 4; ++i)
        {
            auto obstacle = _obstaclePool.RequestObject();
            if (obstacle) _obstacleList.push_back(obstacle);
        }

        // Inicializa los obstáculos
        for (auto & obstacle : _obstacleList)
        {
            obstacle->set_speed_y(0.0f);
            obstacle->set_position_x(canvas_width / 2.0f);
            obstacle->set_position_y(canvas_height);
        }
    }

    void GameScene::RenderSprites(Canvas & canvas)
    {
        // Dibuja el botón de pausa
        if (_pauseButton) _pauseButton->render(canvas);

        // Dibuja los obstáculos
        for (const auto & obstacle : _obstacleList) obstacle->render(canvas);

        // Dibuja el objeto jugador
        _player.RenderPlayer(canvas);
    }

    void GameScene::UpdateSceneObjects(float deltaTime)
    {
        // Llama a update en el _player
        _player.UpdatePlayer(deltaTime, _touchingScreen);

        // Llama a update de los obstáculos
        for (auto & obstacle : _obstacleList)
        {
            obstacle->set_speed_y(_obstaclesDefaultVerticalSpeed);
            obstacle->update(deltaTime);
        }
    }

    void GameScene::RenderPauseMenu(Canvas & canvas)
    {

    }

    void GameScene::CheckForPause(const Point2f & touchPosition)
    {
        // Si el punto dónde ha tocado el punto de usuario pertenece a la caja que conforma el sprite,
        // se cambia el estado a PAUSED
        if (_pauseButton->contains(touchPosition)) state = PAUSED;
    }

}
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
                    { ID(pauseButton),     "high/ui/pause-button.png"         },
            };

    unsigned GameScene::_texturesCount = sizeof(_texturesData) / sizeof(Texture_Data);

    GameScene::GameScene()
    {
        _isAspectRatioAdjusted = false;

        canvas_width  = 1920;
        canvas_height = 1080;

        _pauseButton = nullptr;
    }

    bool GameScene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 0.0f;
        y         = 0.0f;

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
        if (state == RUNNING || state  == PAUSED)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    _isTouchingScreen = true;

                    if (x > canvas_width / 2.0f)  _player.SetDirection(-1.0f);
                    else if (x < canvas_width / 2.0f) _player.SetDirection(1.0f);

                    break;
                }
                case ID(touch-ended):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    _isTouchingScreen = false;

                    Point2f touchPoint = {x, y};    // Crea un punto con ambas coordenadas de la zona de la pantalla donde se ha dejado de pulsar
                    CheckPauseButton(touchPoint);   // Si se ha tocado el botón de pausa, el estado de la escena pasa a ser PAUSED

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
        if (!suspended && state == RUNNING || state == PAUSED)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear();

                if (state == RUNNING)
                {
                    canvas->set_color(0.0f, 0.0f, 0.0f);
                    canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height});

                    RenderSprites(*canvas);
                }
                else if (state == PAUSED)
                {
                    canvas->set_color(0.0f, 0.0f, 0.0f);
                    canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height});

                    RenderPauseMenu(*canvas);
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

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

                InitializeSceneObjects();
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

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::LoadTextures(GameScene::GraphicsContextAccessor & context)
    {

        // Carga el atlas de los elementos de UI
        _uiAtlas.reset (new Atlas("high/ui/ui-spritesheet.sprites", context));

        // Si se ha podido cargar el atlas, se configuran los botones de UI
        if (_uiAtlas->good()) ConfigurePauseMenuOptions();

        // Se ejecuta por cada textura del array de texturas
        if (_textures.size() < _texturesCount)
        {
            // Obtiene la información de las texturas del array de texturas
            Texture_Data & currentTextureData = _texturesData[_textures.size ()];
            std::shared_ptr<Texture_2D> & currentTexture = _textures[currentTextureData.id] = Texture_2D::create (currentTextureData.id, context, currentTextureData.path);

            // Añade la textura al contexto gráfico
            if (currentTexture) context->add (currentTexture);
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

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::CreateSprites()
    {
        // CREACIÓN DE LOS SPRITES DE LA ESCENA
        shared_ptr<Sprite> blueCircle(new Sprite(_textures[ID(blueCircleId)].get()));
        shared_ptr<Sprite> redCircle(new Sprite(_textures[ID(redCircleId)].get()));
        shared_ptr<Sprite> rectangle01(new Sprite(_textures[ID(rect01Id)].get()));
        shared_ptr<Sprite> rectangle02(new Sprite(_textures[ID(rect02Id)].get()));
        shared_ptr<Sprite> rectangle03(new Sprite(_textures[ID(rect03Id)].get()));
        _pauseButton = make_shared<Sprite>(_textures[ID(pauseButton)].get());                 // Botón de pausa externalizado para poder ser controlado

        // Inicializa los puntos de anclaje de los sprites de los obstáculos
        rectangle01->set_anchor(CENTER);
        rectangle02->set_anchor(CENTER);
        rectangle03->set_anchor(CENTER);

        // INICIALIZACIÓN DEL BOTÓN DE PAUSA
        _pauseButton->set_anchor(CENTER);                                                  // Establecimiento del punto de anclaje
        const float xOffset = 10.0f;                                                                 // Offset de la posición
        const float yOffset = 10.0f;
        Point2f pauseButtonPosition = { canvas_width - xOffset, canvas_height - yOffset };
        _pauseButton->set_position(pauseButtonPosition);                                  // Establece la posición

        // Establecimiento de la posición incial de los círculos
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

        // Añade al array de sprites de player los sprites de las bolas
        _player.AddPlayerSprite(blueCircle);
        _player.AddPlayerSprite(redCircle);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    void GameScene::InitializeSceneObjects()
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

    void GameScene::RenderPauseMenu(Canvas & canvas)
    {

    }

    void GameScene::RenderSprites(Canvas & canvas)
    {
        // Dibuja el objeto jugador
        _player.RenderPlayer(canvas);

        // Dibuja los obstáculos
        for (const auto & obstacle : _obstacleList) obstacle->render(canvas);

        // Dibuja la interfaz de usuario
        RenderPauseMenu(canvas);
    }

    void GameScene::UpdateSceneObjects(float deltaTime)
    {
        // Llama a update en el player
        _player.UpdatePlayer(deltaTime, _isTouchingScreen);

        // Llama a update de los obstáculos
        for (auto & obstacle : _obstacleList)
        {
            obstacle->set_speed_y(_obstaclesDefaultVerticalSpeed);
            obstacle->update(deltaTime);
        }
    }

    void GameScene::ConfigurePauseMenuOptions()
    {
        // Se asigna un slice del atlas a cada opción del menú según su ID:

        options[MENU].slice = _uiAtlas->get_slice (ID(menu));
        options[RESUME].slice = _uiAtlas->get_slice (ID(resume));

        // Se calcula la altura total del menú:

        float menu_height = 0;

        for (auto & option : options)menu_height += option.slice->height;

        // Se calcula la posición del borde superior del menú en su conjunto de modo que
        // quede centrado verticalmente:

        float option_top = float(canvas_height) / float(2.f + menu_height / 2.f);

        // Se establece la posición del borde superior de cada opción:

        for (unsigned index = 0; index < number_of_options; ++index)
        {
            options[index].position = Point2f{ canvas_width / 2.f, option_top };

            option_top -= options[index].slice->height;
        }

    }
}
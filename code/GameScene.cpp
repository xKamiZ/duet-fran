/*
 * GAME SCENE
 * AUTHOR: Fran Caamaño Martínez
 */

#include "GameScene.hpp"
#include "MainMenuScene.hpp"

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
        _obstacleList = { };
        _obstaclePool = { };

        _pauseButton = nullptr;
        atlas = nullptr;

        _elapsedSeconds = 0;
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

                    // Si el estado es PAUSED, se comprueba si se pulsa el botón resume para cambiar el estado a RUNNING
                    if (state == PAUSED)
                    {
                        if (OptionAt(touchPosition) == RESUME)
                        {
                            state = RUNNING;
                        }
                        else if (OptionAt(touchPosition) == MENU)
                        {
                            director.run_scene (shared_ptr<Scene>(new MainMenuScene));
                        }
                    }

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
            if (state == RUNNING || state == PAUSED)
            {
                Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

                if (!canvas)
                {
                    canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
                }

                if (canvas)
                {
                    canvas->set_color(0.0f, 0.0f, 0.0f);
                    canvas->fill_rectangle({0.0f, 0.0f}, {(float)canvas_width, (float)canvas_height });

                    switch (state)
                    {
                        case LOADING: break;
                        case RUNNING:

                            canvas->clear();
                            RenderSprites(*canvas);

                            break;

                        case PAUSED:

                            canvas->clear();
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

    void GameScene::LoadTextures(GameScene::GraphicsContextAccessor & context)
    {
        // Se carga el atlas del menú de pausa:
        atlas.reset (new Atlas("high/ui/pause-menu-atlas.sprites", context));

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
                ConfigurePauseMenuOptions();

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
        _obstaclePool.Add(rectangle01.get());
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

        // TEMPORAL: Obtención de los obstáculos del pool
        for (int i = 0; i < 4; ++i)
        {
            auto obstacle = _obstaclePool.RequestObject();
            if (obstacle) _obstacleList.push_back(obstacle);
        }

        // Inicializa los obstáculos
        for (auto & obstacle : _obstacleList)
        {
            // Se genera una posición aleatoria a lo ancho del canvas
            std::random_device generator;
            std::uniform_real_distribution<float> distribution(0.0f, canvas_width);
            float randomXPosition = distribution(generator);

            const float heightOffset = 50.0f;

            obstacle->set_speed_y(0.0f);
            obstacle->set_position_x(randomXPosition);
            obstacle->set_position_y(canvas_height + heightOffset);;
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

        // Llama a update de todos los obstáculos
        for (auto & obstacle : _obstacleList)
        {
            obstacle->set_speed_y(_obstaclesDefaultVerticalSpeed);

            obstacle->update(deltaTime);
        }
    }

    void GameScene::ConfigurePauseMenuOptions()
    {
        // Se asigna un slice del atlas a cada opción del menú según su ID:

        options[RESUME].slice = atlas->get_slice (ID(resume));
        options[MENU].slice = atlas->get_slice (ID(menu));

        // Se calcula la altura total del menú:

        float menu_height = 0.0f;

        for (auto & option : options) menu_height += option.slice->height;

        // Se calcula la posición del borde superior del menú en su conjunto de modo que
        // quede centrado verticalmente:

        float option_top = canvas_height / 2.f + menu_height / 2.f;

        // Se establece la posición del borde superior de cada opción:

        for (unsigned index = 0; index < number_of_options; ++index)
        {
            options[index].position = Point2f{ canvas_width / 2.f, option_top };

            option_top -= options[index].slice->height;
        }

        for (auto & option : options)
        {
            option.is_pressed = false;
        }
    }

    void GameScene::RenderPauseMenu(Canvas & canvas)
    {
        // Se dibuja el slice de cada una de las opciones del menú:

        for (auto & option : options)
        {
            canvas.set_transform
                    (
                            scale_then_translate_2d
                                    (
                                            option.is_pressed ? 0.75f : 1.f,              // Escala de la opción
                                            { option.position[0], option.position[1] }      // Traslación
                                    )
                    );

            canvas.fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
        }
    }

    void GameScene::CheckForPause(const Point2f & touchPosition)
    {
        // Si el punto dónde ha tocado el punto de usuario pertenece a la caja que conforma el sprite,
        // se cambia el estado a PAUSED
        if (state == RUNNING && _pauseButton->contains(touchPosition))
        {
            state = PAUSED;
        }
    }

    int GameScene::OptionAt (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
                    (
                    point[0] > option.position[0] - option.slice->width  &&
                    point[0] < option.position[0] + option.slice->width  &&
                    point[1] > option.position[1] - option.slice->height &&
                    point[1] < option.position[1] + option.slice->height
                    )
            {
                return index;
            }
        }

        return -1;
    }
}
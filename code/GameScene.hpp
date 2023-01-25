/*
 * GAME SCENE
 * AUTHOR: Fran Caamaño Martínez
 */

#include <map>
#include <list>
#include <memory>
#include <vector>

#include <basics/Canvas>
#include <basics/Id>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>
#include <basics/Director>
#include <basics/Vector>

#include "Sprite.hpp"
#include "Player.hpp"
#include "ObjectPool.hpp"

namespace DuetClone
{

    class GameScene : public basics::Scene
    {

        typedef std::map<basics::Id, std::shared_ptr<Texture_2D>> Texture_Map;
        typedef basics::Graphics_Context::Accessor GraphicsContextAccessor;

        enum Option_Id
        {
            MENU,
            RESUME
        };

        struct UIOption
        {
            const basics::Atlas::Slice * slice;
            Point2f position;
            float   is_pressed;
        };

        static const unsigned number_of_options = 3;

    private:

        static struct Texture_Data { basics::Id id; const char * path; } _texturesData[];
        static unsigned _texturesCount;                             // Número de texturas de _texturesData
        float _obstaclesDefaultVerticalSpeed = -150.0f;             // Velocidad de movimiento vertical de los obstáculos
                                                                    // No es constante porque hay obstáculos que se desplazarán más rápido

        basics::Timer _timer;
        Texture_Map  _textures;                                     // Diccionario que contiene punteros a las texturas de los objetos
        Player _player;                                              // Objeto jugador
        ObjectPool<Sprite> _obstaclePool;                           // Pool de obstáculos (sprites de rectángulos)
        vector<Sprite*> _obstacleList;                              // Contenedor de obstáculos que se van sacando del pool
        std::list<shared_ptr<Sprite>> _spriteList;
        bool _isTouchingScreen;                                     // Indica si la pantalla está siendo pulsada
        std::unique_ptr<basics::Atlas> _uiAtlas;                    // Atlas que contiene las texturas de los elementos de UI
        UIOption options[number_of_options];                        // Datos de las opciones de la UI
        shared_ptr<Sprite> _pauseButton;                            // Sprite del botón de pausa

    public:

        enum State
        {
            LOADING,
            RUNNING,
            PAUSED,
        };

        State state;
        bool suspended;

        bool _isAspectRatioAdjusted;                                // Indica si está ajustado o no el Aspect Ratio

        unsigned canvas_width;
        unsigned canvas_height;

        float x, y;

    public:

        GameScene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();                                                    // Se ejecuta en Update cuando el estado es LOADING
        void run  (float deltaTime);                                     // Se ejecuta en Update cuando el estado es RUNNING

        void AdjustAspectRatio(GraphicsContextAccessor & context);       // Ajusta el aspect ratio al real de la pantalla
        void LoadTextures(GraphicsContextAccessor & context);            // Carga las texturas
        void CreateSprites();                                            // Crea los Sprites que habrá en la escena una vez las texturas hayan sido cargadas
        void RenderSprites(basics::Canvas & canvas);                     // Dibuja los sprites de la escena de juego
        void UpdateSceneObjects(float deltaTime);                        // Actualiza los objetos de la escena de juego (se llama en run)
        void InitializeSceneObjects();                                         // Inicializa los objetos de la escena que lo requieran (se llama en load)
        void RenderPauseMenu(basics::Canvas & canvas);                   // Dibuja los elementos de interfaz de usuario
        void ConfigurePauseMenuOptions();                                // Configura las opciones de la UI una vez cargado el atlas de elementos de UI
        inline void CheckPauseButton(Point2f touchPoint) { if (_pauseButton->contains(touchPoint)) state = PAUSED; }  // Establece el valor de _gamePaused
    };
}
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

    private:

        static struct   Texture_Data { basics::Id id; const char * path; } _texturesData[];
        static unsigned _texturesCount;
        float _obstaclesDefaultVerticalSpeed = -150.0f;             // Velocidad de movimiento vertical de los obstáculos
                                                                    // No es constante porque hay obstáculos que se desplazarán más rápido

        basics::Timer _timer;

        Texture_Map  _textures;                                     // Diccionario que contiene punteros a las texturas de los objetos

        Player _player;                                              // Objeto jugador

        ObjectPool<Sprite> _obstaclePool;                           // Pool de obstáculos (sprites de rectángulos)
        vector<Sprite*> _obstacleList;                              // Contenedor de obstáculos que se van sacando del pool

        std::list<shared_ptr<Sprite>> _spriteList;

        bool _touchingScreen;


    public:

        enum State
        {
            LOADING,
            RUNNING,
            PAUSED,
        };

        State state;
        bool suspended;

        bool _isAspectRatioAdjusted;     // Indica si está ajustado o no el Aspect Ratio

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

        void load ();
        void run  (float time);

        void AdjustAspectRatio(GraphicsContextAccessor & context);       // Ajusta el aspect ratio al real de la pantalla
        void LoadTextures(GraphicsContextAccessor & graphicsContext);    // Carga las texturas
        void CreateSprites();                                            // Crea los Sprites que habrá en la escena una vez las texturas hayan sido cargadas
        void RenderSprites(basics::Canvas & canvas);                     // Dibuja los sprites de la escena de juego
        void UpdateSceneObjects(float deltaTime);                        // Actualiza los objetos de la escena de juego (se llama en run)
        void InitSceneObjects();                                         // Inicializa los objetos de la escena que lo requieran (se llama en load)
        void RenderPauseMenu(basics::Canvas & canvas);                   // Dibuja en pantalla el menú de pausa
        void CheckForPause(const Point2f & touchPosition);               // Comprueba si touchPosition pertence al sprite del botón de pausa
    };
}
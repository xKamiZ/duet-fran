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

namespace DuetClone
{

    class GameScene : public basics::Scene
    {

        typedef std::shared_ptr<Sprite> Sprite_Pointer;
        typedef std::list<Sprite_Pointer> Sprite_List;

        typedef std::shared_ptr<Texture_2D> Texture_Pointer;
        typedef std::map<basics::Id, Texture_Pointer> Texture_Map;

        typedef basics::Graphics_Context::Accessor GraphicsContextAccessor;

    private:

        static struct   Texture_Data { basics::Id id; const char * path; } _texturesData[];
        static unsigned _texturesCount;

        basics::Timer _timer;

        Texture_Map  _textures;           // Diccionario que contiene punteros a las texturas de los objetos
        Sprite_List _sprites;             // Lista de punteros a sprites que contiene los sprites de los elementos de la escena

        Player player;                    // Objeto jugador
        Player * _playerPtr = nullptr;    // Puntero al jugador

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State state;
        bool suspended;

        bool _isAspectRatioAdjusted;            // Indica si está ajustado o no el Aspect Ratio

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
        void UpdateSceneObjects(float deltaTime);                        // Actualiza los objetos de la escena de juego

    };

}

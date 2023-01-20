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

namespace DuetClone
{

    class GameScene : public basics::Scene
    {

        typedef std::shared_ptr<Sprite> Sprite_Pointer;
        typedef std::list<Sprite_Pointer> Sprite_List;

        typedef std::shared_ptr<Texture_2D> Texture_Pointer;
        typedef std::map<basics::Id, Texture_Pointer> Texture_Map;

        typedef basics::Graphics_Context::Accessor GraphicsContext;

    private:

        static struct   Texture_Data { basics::Id id; const char * path; } _texturesData[];
        static unsigned _texturesCount;
        static constexpr float _playerMovementSpeed = 80.0f;

        basics::Timer _timer;

        Texture_Map  _textures;         // Diccionario que contiene punteros a las texturas de los objetos
        Sprite_List _sprites;           // Lista de punteros a sprites que contiene los sprites de los elementos de la escena

        Sprite * _blueCircleSprite;       // Puntero al Sprite de _sprites del barco del jugador
        Sprite * _redCircleSprite;

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        float          x, y;

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

        void LoadTextures(GraphicsContext & graphicsContext);    // Carga las texturas
        void CreateSprites();                                    // Crea los Sprites que habrá en la escena una vez las texturas hayan sido cargadas

        void RenderSprites(basics::Canvas & canvas);             // Dibuja los sprites del juego
        void UpdateSprites(float deltaTime);                     // Llama a update en los sprites del juego

    };

}

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

    GameScene::GameScene()
    {
        canvas_width  = 1920;
        canvas_height =  1080;

        _blueCircleSprite = nullptr;
        _redCircleSprite = nullptr;
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
                    case LOADING: break;
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
        if (_textures.size() < _texturesCount)
        {

            Texture_Data   & currentTextureData = _texturesData[_textures.size ()];
            Texture_Pointer & currentTexture = _textures[currentTextureData.id] = Texture_2D::create (currentTextureData.id, graphicsContext, currentTextureData.path);

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

        _sprites.push_back(blueCircle);
        _sprites.push_back(redCircle);

        _blueCircleSprite = blueCircle.get();
        _redCircleSprite = redCircle.get();
    }

    void GameScene::RenderSprites(Canvas &canvas)
    {
        for (auto & sprite : _sprites) sprite->render(canvas);
    }

    void GameScene::UpdateSprites(float deltaTime)
    {
        for (auto & sprite : _sprites) sprite->update(deltaTime);
    }

}

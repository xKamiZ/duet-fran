/*
 * INTRO SCENE
 * AUTHOR: Fran Caamaño Martínez
 */

#include "IntroScene.hpp"

#include "MainMenuScene.hpp"
#include <basics/Canvas>
#include <basics/Director>


namespace DuetClone
{

    bool IntroScene::initialize ()
    {
        if (state == UNINITIALIZED)
        {
            state = LOADING;
        }
        else
        {
            timer.reset ();

            opacity = 0.f;
            state   = FADING_IN;
        }
        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void IntroScene::update (float time)
    {
        if (!suspended) switch (state)
            {
                case LOADING:    update_loading    (); break;
                case FADING_IN:  update_fading_in  (); break;
                case WAITING:    update_waiting    (); break;
                case FADING_OUT: update_fading_out (); break;
                default: break;
            }
    }

    // ---------------------------------------------------------------------------------------------

    void IntroScene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();

                if (logo_texture)
                {
                    canvas->set_opacity (opacity);

                    canvas->fill_rectangle
                            (
                                    { canvas_width * .5f, canvas_height * .5f },
                                    {logo_texture->get_width (), logo_texture->get_height () },
                                    logo_texture. get ()
                            );
                }
            }
        }
    }

    void IntroScene::update_loading ()
    {
        Graphics_Context::Accessor context = basics::director.lock_graphics_context ();

        if (context)
        {
            if (!_isAspectRatioAdjusted) AdjustAspectRatio(context);

            // Se carga la textura del icono para la intro:

            logo_texture = Texture_2D::create (0, context, "logo.png");

            // Se comprueba si la textura se ha podido cargar correctamente:

            if (logo_texture)
            {
                context->add (logo_texture);

                timer.reset ();

                opacity = 0.f;
                state   = FADING_IN;
            }
            else
                state   = ERROR;
        }
    }

    void IntroScene::update_fading_in ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < 1.f)
        {
            opacity = elapsed_seconds;      // Se aumenta la opacidad del logo a medida que pasa el tiempo
        }
        else
        {
            timer.reset ();

            opacity = 1.f;
            state   = WAITING;
        }
    }

    void IntroScene::update_waiting ()
    {
        // Se esperan dos segundos sin hacer nada:

        if (timer.get_elapsed_seconds () > 2.f)
        {
            timer.reset ();

            state = FADING_OUT;
        }
    }

    void IntroScene::update_fading_out ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = 1.f - elapsed_seconds * 2.f;      // Se reduce la opacidad de 1 a 0 en medio segundo
        }
        else
        {
            // Cuando el faceout se ha completado, se lanza la siguiente escena:

            state = FINISHED;

            basics::director.run_scene (std::shared_ptr<Scene>(new MainMenuScene));
        }
    }

    void IntroScene::AdjustAspectRatio(Graphics_Context::Accessor & context)
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

} // DuetClone
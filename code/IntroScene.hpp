/*
 * INTRO SCENE
 * AUTHOR: Fran Caamaño Martínez
 */


#ifndef BASICS_PROJECT_TEMPLATE_INTROSCENE_HPP
#define BASICS_PROJECT_TEMPLATE_INTROSCENE_HPP

#include <memory>
#include <basics/Canvas>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>

using basics::Timer;
using basics::Canvas;
using basics::Texture_2D;
using basics::Graphics_Context;

namespace DuetClone
{

    class IntroScene : public basics::Scene
    {

        /**
         * Representa el estado de la escena en su conjunto.
         */
        enum State
        {
            UNINITIALIZED,
            LOADING,
            FADING_IN,
            WAITING,
            FADING_OUT,
            FINISHED,
            ERROR
        };

    private:

        State    state;                                     ///< Estado de la escena.
        bool     suspended;                                 ///< true cuando la escena está en segundo plano y viceversa.

        unsigned canvas_width;                              ///< Ancho de la resolución virtual usada para dibujar.
        unsigned canvas_height;                             ///< Alto  de la resolución virtual usada para dibujar.

        Timer    timer;                                     ///< Cronómetro usado para medir intervalos de tiempo.

        float    opacity;                                   ///< Opacidad de la textura.

        std::shared_ptr < Texture_2D > logo_texture;        ///< Textura que contiene la imagen del logo.

        bool _isAspectRatioAdjusted;     // Indica si está ajustado o no el Aspect Ratio

    public:

        IntroScene()
        {
            _isAspectRatioAdjusted = false;

            state         = UNINITIALIZED;
            suspended     = true;
            canvas_width  = 1920;
            canvas_height =  1080;
        }

        /**
         * Este método lo llama Director para conocer la resolución virtual con la que está
         * trabajando la escena.
         * @return Tamaño en coordenadas virtuales que está usando la escena.
         */
        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        /**
         * Aquí se inicializan los atributos que deben restablecerse cada vez que se inicia la escena.
         * @return
         */
        bool initialize () override;

        /**
         * Este método lo invoca Director automáticamente cuando el juego pasa a segundo plano.
         */
        void suspend () override
        {
            suspended = true;
        }

        /**
         * Este método lo invoca Director automáticamente cuando el juego pasa a primer plano.
         */
        void resume () override
        {
            suspended = false;
        }

        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * actualize su estado.
         */
        void update (float time) override;

        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * dibuje su contenido.
         */
        void render (Graphics_Context::Accessor & context) override;

    private:

        void update_loading    ();
        void update_fading_in  ();
        void update_waiting    ();
        void update_fading_out ();
        void AdjustAspectRatio(basics::Graphics_Context::Accessor & context);       // Ajusta el aspect ratio al real de la pantalla
    };

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_INTROSCENE_HPP

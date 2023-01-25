/*
 * MAIN MENU SCENE
 * AUTHOR: Fran Caamaño Martínez
 */

#ifndef BASICS_PROJECT_TEMPLATE_MAINMENUSCENE_HPP
#define BASICS_PROJECT_TEMPLATE_MAINMENUSCENE_HPP

#include <memory>
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Point>
#include <basics/Scene>
#include <basics/Size>
#include <basics/Timer>

using basics::Atlas;
using basics::Timer;
using basics::Canvas;
using basics::Point2f;
using basics::Size2f;
using basics::Texture_2D;
using basics::Graphics_Context;

namespace DuetClone
{

    class MainMenuScene : public basics::Scene{


        /**
        * Representa el estado de la escena en su conjunto.
        */
        enum State
        {
            LOADING,
            READY,
            FINISHED,
            ERROR
        };

        enum Option_Id
        {
            PLAY,
            HELP,
            EXIT
        };

        struct Option
        {
            const Atlas::Slice * slice;
            Point2f position;
            float   is_pressed;
        };


        static const unsigned number_of_options = 3;

    private:

        State    state;                                     ///< Estado de la escena.
        bool     suspended;                                 ///< true cuando la escena está en segundo plano y viceversa.

        unsigned canvas_width;                              ///< Ancho de la resolución virtual usada para dibujar.
        unsigned canvas_height;                             ///< Alto  de la resolución virtual usada para dibujar.

        Timer    timer;                                     ///< Cronómetro usado para medir intervalos de tiempo.

        Option   options[number_of_options];                ///< Datos de las opciones del menú

        std::unique_ptr< Atlas > atlas;                     ///< Atlas que contiene las imágenes de las opciones del menú

        bool _isAspectRatioAdjusted;     // Indica si está ajustado o no el Aspect Ratio

    public:

        MainMenuScene();

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
         * Este método se invoca automáticamente una vez por fotograma cuando se acumulan
         * eventos dirigidos a la escena.
         */
        void handle (basics::Event & event) override;

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

        /**
         * Establece las propiedades de cada opción si se ha podido cargar el atlas.
         */
        void configure_options ();

        /**
         * Devuelve el índice de la opción que se encuentra bajo el punto indicado.
         * @param point Punto que se usará para determinar qué opción tiene debajo.
         * @return Índice de la opción que está debajo del punto o -1 si no hay alguna.
         */
        int option_at (const Point2f & point);

        void AdjustAspectRatio(basics::Graphics_Context::Accessor & context);       // Ajusta el aspect ratio al real de la pantalla
    };

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_MAINMENUSCENE_HPP

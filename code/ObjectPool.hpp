/*
 * OBJECT POOL CLASS TEMPLATE
 * AUTHOR: Fran Caamaño Martínez
 */

#pragma once
#ifndef BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP
#define BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

#include <unordered_map>

using std::vector;
using std::pair;

namespace DuetClone {

    template<typename T>
    class ObjectPool {

        typedef std::unordered_map<T*, bool> Pool;

    public:

        ObjectPool(const vector<T> & collection);

    private:

        Pool _pool;                                                             // Pool de objetos
                                                                                // Cada clave del diccionario tendrá
                                                                                // asignado como valor el estado en
                                                                                // el que se encuentra (activo o inactivo)
                                                                                // True indica que está en uso y False que no está en uso

    public:

        T & RequestObject();                                                    // Retorna una referencia al objeto correspondiente del pool

    };


    template<typename T>
    T & ObjectPool<T>::RequestObject()
    {
        // Itera por el pool en búsqueda de un elemento no activo
        for (auto & element : _pool)
        {
            // Si el elemento no está activo, lo activa y retorna su referencia
            if (element.second == false)
            {
                element.second == true;
                static auto returnedElement = element; // Creamos una variable static con la misma referencia
                                                         // para que no retorne una referencia colgante

                return returnedElement;
            }
        }

        // Si se finaliza el bucle sin encontrar un elemento disponible
        // crear un nuevo elemento al final del diccionario y retornarlo


    }

    template<typename T>
    ObjectPool<T>::ObjectPool(const vector <T> & collection)
    {
        // Recorre la colección de objetos e inserta cada elemento
        // en la pool estableciendo como estado inicial false
        for (auto & element : collection) _pool.insert(pair<T*, bool>(element, false));
    }
} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

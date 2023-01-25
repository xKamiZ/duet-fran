/*
 * OBJECT POOL CLASS TEMPLATE
 * AUTHOR: Fran Caamaño Martínez
 */

#pragma once
#ifndef BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP
#define BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

#include <map>
#include <memory>

using std::vector;
using std::pair;

namespace DuetClone {

    template<typename T>
    class ObjectPool {

        typedef std::map<T*, bool> Pool;

    private:

        Pool _pool;

    public:

        T * RequestObject();            // Retorna un objeto T disponible en el pool
        void Add(T * object);           // Añade una nueva referencia al pool de objetos
    };


    template<typename T>
    T * ObjectPool<T>::RequestObject()
    {

        // Itera por el pool en búsqueda de un elemento no activo
        for (auto & object : _pool)
        {
            // Si el elemento no está activo, lo activa y retorna su referencia
            if (object.second == false)
            {
                object.second = true; // Cambia el estado de uso

                return object.first; // Retorna el puntero al objeto disponible
            }
        }

        // Si se finaliza el bucle sin encontrar un elemento disponible...

        return nullptr;

    }

    template<typename T>
    void ObjectPool<T>::Add(T * object)
    {
        _pool.insert(pair<T*, bool>(object, false)); // Añade al pool un nuevo objeto
    }

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

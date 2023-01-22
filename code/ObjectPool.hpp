/*
 * OBJECT POOL CLASS TEMPLATE
 * AUTHOR: Fran Caamaño Martínez
 */

#ifndef BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP
#define BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

#include <vector>

using std::vector;

namespace DuetClone {

    template<typename T>
    class ObjectPool {

    public:

        ObjectPool(const T & objectType)
        {
            _poolType = objectType; // Inicializa la referencia al tipo de objeto de la pool
        }

    private:

        vector<T> _pool;                                                        // Pool de objetos
        const T & _poolType;                                                    // Referencia al tipo de objeto del pool

    public:

        T & RequestElement();                                                    // Retorna una referencia al objeto correspondiente del pool
        void AddElement(const T & newElement) {  _pool.push_back(newElement); }  // Añade un nuevo elemento a la pool

    };

    template<typename T>
    T & ObjectPool<T>::RequestElement()
    {
        const T * lastElement = _pool.end();
        for (T * currentElement = _pool.begin(); currentElement < lastElement; ++currentElement)
        {
            // comprobar si elemento no activo
            // si no activo, activa elemento y retorna
            // si no, continúa
            // si todos los elementos activos, incrementa tamaño pool
        }
    }


} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_OBJECTPOOL_HPP

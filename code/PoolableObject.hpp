/*
 * POOLABLE OBJECT CLASS
 * AUTHOR: Fran Caamaño Martínez
 */

#ifndef BASICS_PROJECT_TEMPLATE_POOLABLEOBJECT_HPP
#define BASICS_PROJECT_TEMPLATE_POOLABLEOBJECT_HPP

namespace DuetClone {

    class PoolableObject {

    public:

        PoolableObject()
        {
            _inUse = true;
        }

        bool InUse() { return _inUse; };
        void SetInUse(bool newState) {_inUse = newState; }

    private:

        bool _inUse;

    };

} // DuetClone

#endif //BASICS_PROJECT_TEMPLATE_POOLABLEOBJECT_HPP

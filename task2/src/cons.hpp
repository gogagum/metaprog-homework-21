//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_CONS_HPP
#define TASK2_CONS_HPP

#include "type_lists_concepts.hpp"

namespace TypeLists {
    /*
     * Cons<T, TL>
     * -----------
     * Add element T into beginning of TypeList TL.
     */
    template <class T, TypeList TL>
    struct Cons {
        using Head = T;
        using Tail = TL;
    };
}

#endif //TASK2_CONS_HPP

//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_ZIP2_TO_LISTS_HPP
#define TASK2_ZIP2_TO_LISTS_HPP

#include "type_lists_concepts.hpp"
#include "cons.hpp"

namespace TypeLists {
    /*
     * Zip22Lists, zip with every pair as TypeList
     */
    template <TypeList TL1, TypeList TL2>
    struct Zip22Lists : public Nil {};

    template <TypeSequence TS1, TypeSequence TS2>
    struct Zip22Lists<TS1, TS2> {
        using Head = Cons<typename TS1::Head, Cons<typename TS2::Head, Nil>>;
        using Tail = Zip22Lists<typename TS1::Tail, typename TS2::Tail>;
    };
}

#endif //TASK2_ZIP2_TO_LISTS_HPP

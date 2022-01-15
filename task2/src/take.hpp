//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_TAKE_HPP
#define TASK2_TAKE_HPP

#include "type_lists_concepts.hpp"

namespace TypeLists {
    /*
     * Take<N, TL>
     * -----------
     * Take first N types from TypeList TL
     */
    template <std::size_t N, TypeList TL>
    struct Take {
        using Head = typename TL::Head;
        using Tail = Take<N-1, typename TL::Tail>;
    };

    template <std::size_t N, Empty TE>
    struct Take<N, TE> : public Nil {};

    template <TypeList TL>
    struct Take<0, TL> : public Nil {};
}

#endif //TASK2_TAKE_HPP

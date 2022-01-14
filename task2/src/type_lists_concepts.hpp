//
// Created by gogagum on 13.01.2022.
//

#ifndef TASK2_TYPE_LISTS_CONCEPTS_HPP
#define TASK2_TYPE_LISTS_CONCEPTS_HPP

#include <concepts>

namespace TypeLists {
// TypeSequence - концепт из условия.
    template<class TL>
    concept TypeSequence =
    requires {
        typename TL::Head;
        typename TL::Tail;
    };

/*
 * Nil конец TypeList`а
 */
    struct Nil {};

// concept Empty - концепт из условия, пустой TypeList.
    template<class TL>
    concept Empty = std::derived_from<TL, Nil>;

// concept TypeList - концепт из условия.
    template<class TL>
    concept TypeList = Empty<TL> || TypeSequence<TL>;

}

#endif //TASK2_TYPE_LISTS_CONCEPTS_HPP

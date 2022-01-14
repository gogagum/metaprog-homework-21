//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_MAP_HPP
#define TASK2_MAP_HPP

#include "type_lists_concepts.hpp"

namespace TypeLists {
    /*
     * Map<F, TL>
     * ----------
     * Apply F to all types of TypeList TL.
     */
    template <template<typename> class F, TypeList TL>
    struct Map : public Nil {};

    template <template<typename> class F, TypeSequence TS>
    struct Map<F, TS> {
        using Head = F<typename TS::Head>;
        using Tail = Map<F, typename TS::Tail>;
    };
}

#endif //TASK2_MAP_HPP

//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_CYCLE_HPP
#define TASK2_CYCLE_HPP

#include "type_lists_concepts.hpp"

namespace TypeLists {
    /*
     * Cycle
     */
    template <TypeList TL>
    struct Cycle : Nil {};

    template <TypeSequence TS>
    struct Cycle<TS>{
    private:
        template <TypeList CurrTL>
        struct CycleImpl {
            using Head = typename TS::Head;
            using Tail = typename CycleImpl<TS>::Tail;
        };

        template <TypeSequence CurrTL>
        struct CycleImpl<CurrTL>{
            using Head = typename CurrTL::Head;
            using Tail = CycleImpl<typename CurrTL::Tail>;
        };

        using CycleRet = CycleImpl<TS>;

    public:

        using Head = typename CycleRet::Head;
        using Tail = typename CycleRet::Tail;
    };
}

#endif //TASK2_CYCLE_HPP

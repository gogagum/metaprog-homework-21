//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_INITS_HPP
#define TASK2_INITS_HPP

#include "type_lists_concepts.hpp"
#include "take.hpp"

namespace TypeLists {
    /*
     * Inits
     */
    template <TypeList TL>
    struct Inits {
        using Head = Nil;
        using Tail = Nil;
    };

    template <TypeSequence TS>
    struct Inits<TS> {
    private:

        template <std::size_t N, TypeList Tail>
        struct InitsImpl;

        template <std::size_t N, Empty TE>
        struct InitsImpl<N, TE> {
            using Head = Take<N, TS>;
            using Tail = Nil;
        };

        template <std::size_t N, TypeSequence TS_>
        struct InitsImpl<N, TS_> {
            using Head = Take<N, TS>;
            using Tail = InitsImpl<N + 1, typename TS_::Tail>;
        };

        using InitsRet = InitsImpl<0, TS>;
    public:
        using Head = typename InitsRet::Head;
        using Tail = typename InitsRet::Tail;
    };
}

#endif //TASK2_INITS_HPP

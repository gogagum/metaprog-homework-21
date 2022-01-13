//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_DROP_HPP
#define TASK2_DROP_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    template <std::size_t N, TypeList TL>
    struct DropImpl;

    template <TypeList TL>
    struct DropImpl<0, TL> {
        using Ret = TL;
    };

    template <std::size_t N, Empty TE>
    struct DropImpl<N, TE> {
        using Ret = Nil;
    };

    template <std::size_t N, TypeSequence TS>
    struct DropImpl<N, TS> {
        using Ret = typename DropImpl<N - 1, typename TS::Tail>::Ret;
    };
}

#endif //TASK2_DROP_HPP

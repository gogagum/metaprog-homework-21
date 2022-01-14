//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_DROP_HPP
#define TASK2_DROP_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    /**
     * DropImpl - implementation of Drop metafunction.
     * @tparam N - current drop index.
     * @tparam TL - current TypeList tail.
     */
    template <std::size_t N, TypeList TL>
    struct DropImpl {
        using Ret = Nil;
    };

    template <TypeList TL>
    struct DropImpl<0, TL> {
        using Ret = TL;
    };

    template <std::size_t N, TypeSequence TS>
    struct DropImpl<N, TS> {
        using Ret = typename DropImpl<N - 1, typename TS::Tail>::Ret;
    };
}

#endif //TASK2_DROP_HPP

//
// Created by gogagum on 15.01.2022.
//

#ifndef TASK2_SKIP_EVERY_NTH_HPP
#define TASK2_SKIP_EVERY_NTH_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {

    template<std::size_t n, std::size_t i, TypeList TL>
    struct SkipEveryNthImpl;

    template<std::size_t i, TypeList TL>
    struct SkipEveryNthImpl<0, i, TL> {
        using Ret = Nil;
    };

    template<std::size_t n, std::size_t i, TypeSequence TS> requires (n != i + 1)
    struct SkipEveryNthImpl<n, i, TS> {
        struct Ret {
            using Head = typename TS::Head;
            using Tail =
                typename SkipEveryNthImpl<n, i + 1, typename TS::Tail>::Ret;
        };
    };

    template<std::size_t n, std::size_t i, TypeSequence TS> requires (n == i + 1)
    struct SkipEveryNthImpl<n, i, TS> {
        using Ret = typename SkipEveryNthImpl<n, 0, typename TS::Tail>::Ret;
    };

    template<std::size_t n, std::size_t i, Empty TE>
    struct SkipEveryNthImpl<n, i, TE> {
        using Ret = Nil;
    };
}

#endif //TASK2_SKIP_EVERY_NTH_HPP

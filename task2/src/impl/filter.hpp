//
// Created by gogagum on 14.01.2022.
//

#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TASK2_FILTER_HPP
#define TASK2_FILTER_HPP

#include "../type_lists_concepts.hpp"
#include "skip_while.hpp"
#include "logic_not.hpp"

namespace TypeLists::Impl {
    /**
     * Implementation of Filter metafunction.
     * @tparam P - boolean-return metafunction.
     * @tparam TL - TypeList.
     */
    template <template<typename> class P, TypeList TL>
    struct FilterImpl{
        using Ret = Nil;
    };

    template <template<typename> class P, TypeSequence TS>
    struct FilterImpl<P, TS> {
    private:
        template <typename T>
        using NotP = ApplyLogicNotToValue<P<T>>;

        using Skipped = typename Impl::SkipWhileImpl<NotP, TS>::Ret;  // Skip all args for which P is false.

        template <TypeList TL_>
        struct AfterSkip : Nil {};

        template <TypeSequence TS_>
        struct AfterSkip<TS_> {
            using Head = typename TS_::Head;
            using Tail = typename FilterImpl<P, typename TS_::Tail>::Ret;
        };
    public:
        using Ret = AfterSkip<Skipped>;
    };
}
#endif //TASK2_FILTER_HPP

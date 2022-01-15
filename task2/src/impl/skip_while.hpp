//
// Created by gogagum on 14.01.2022.
//

#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TASK2_SKIP_WHILE_HPP
#define TASK2_SKIP_WHILE_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    /**
     * SkipWhile implementation.
     * @tparam P - single argument template boolean function.
     * @tparam TL - current type list to skip elements.
     */
    template <template <typename> class P, TypeList TL>
    struct SkipWhileImpl {
        using Ret = Nil;
    };

    template <template <typename> class P, TypeSequence TS>
    struct SkipWhileImpl<P, TS> {
      private:
        template <bool Skip, typename _>
        struct Decision {
            using Ret = TS;
        };

        template <typename _>
        struct Decision<false, _> {
            using Ret = typename SkipWhileImpl<P, typename TS::Tail>::Ret;
        };

      public:
        using Ret = typename Decision<!P<typename TS::Head>::Value, TS>::Ret;
    };
}

#endif //TASK2_SKIP_WHILE_HPP

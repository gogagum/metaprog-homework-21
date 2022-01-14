//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_FOLDL_HPP
#define TASK2_FOLDL_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    /**
     * Fold implementation.
     * @tparam OP - metafunction with two type parameters.
     * @tparam T - first element.
     * @tparam TL - other elements.
     */
    template <template <typename, typename> class OP, typename T, TypeList TL>
    struct FoldlImpl {
        using Ret = T;
    };

    template <template<typename, typename> class OP, typename T, TypeSequence TS>
    struct FoldlImpl<OP, T, TS> {
        using Ret =
            typename FoldlImpl<OP,
                               OP<T, typename TS::Head>,
                               typename TS::Tail>::Ret;
    };
}

#endif //TASK2_FOLDL_HPP

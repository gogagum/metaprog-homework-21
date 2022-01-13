//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_GET_FROM_TYPELIST_HPP
#define TASK2_GET_FROM_TYPELIST_HPP

#include <cstddef>
#include "../type_lists_concepts.hpp"
#include "error_return.hpp"

namespace TypeLists::Impl {
    template <std::size_t N, TypeList TL>
    struct GetFromTypeListImpl;

    template <std::size_t N, TypeSequence TS>
    struct GetFromTypeListImpl<N, TS> {
        using Ret = typename GetFromTypeListImpl<N - 1, typename TS::Tail>::Ret;
    };

    template <TypeSequence TS>
    struct GetFromTypeListImpl<0, TS> {
        using Ret = typename TS::Head;
    };

    template <std::size_t N, TypeList TL>
    struct GetFromTypeListWithErrorReturnImpl;

    template <std::size_t N, TypeSequence TS>
    struct GetFromTypeListWithErrorReturnImpl<N, TS> {
        using Ret = typename GetFromTypeListWithErrorReturnImpl<N - 1, typename TS::Tail>::Ret;
    };

    template <TypeSequence TS>
    struct GetFromTypeListWithErrorReturnImpl<0, TS> {
        using Ret = typename TS::Head;
    };

    template <std::size_t N, Empty TE>
    struct GetFromTypeListWithErrorReturnImpl<N, TE> {
        using Ret = ErrorReturn;
    };
}  // namespace TypeLists::Impl

#endif //TASK2_GET_FROM_TYPELIST_HPP

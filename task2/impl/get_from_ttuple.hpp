//
// Created by gogagum on 13.01.2022.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TASK2_GET_FROM_TTUPLE_HPP
#define TASK2_GET_FROM_TTUPLE_HPP

#include "../type_lists_concepts.hpp"
#include "../type_tuples.hpp"

namespace TypeLists::Impl {

    struct ErrorReturn {};
    /*
     * GetFromTTupleImpl<N, Types...>
     */
    template <std::size_t N, typename... Types>
    struct GetFromTTupleImpl;

    template <std::size_t N, typename FirstType, typename... OtherTypes>
    struct GetFromTTupleImpl<N, TypeTuples::TTuple<FirstType, OtherTypes...>> {
        using Ret = typename GetFromTTupleImpl<N - 1, TypeTuples::TTuple<OtherTypes...>>::Ret;
    };

    template <typename FirstType, typename... OtherTypes>
    struct GetFromTTupleImpl<0, TypeTuples::TTuple<FirstType, OtherTypes...>> {
    using Ret = FirstType;
    };

    template <typename OnlyType>
    struct GetFromTTupleImpl<0, TypeTuples::TTuple<OnlyType>> {
    using Ret = OnlyType;
    };

    /*
     * GetFromTTupleWithErrorReturnImpl<N, Types...>
     */
    template <std::size_t N, typename... Types>
    struct GetFromTTupleWithErrorReturnImpl;

    template <std::size_t N, typename FirstType, typename... OtherTypes>
    struct GetFromTTupleWithErrorReturnImpl<N, TypeTuples::TTuple<FirstType, OtherTypes...>> {
        using Ret = typename GetFromTTupleWithErrorReturnImpl<N - 1, TypeTuples::TTuple<OtherTypes...>>::Ret;
    };

    template <typename FirstType, typename... TTupleTypes>
    struct GetFromTTupleWithErrorReturnImpl<0, TypeTuples::TTuple<FirstType, TTupleTypes...>> {
        using Ret = FirstType;
    };

    template <std::size_t N, typename OnlyType>
    struct GetFromTTupleWithErrorReturnImpl<N, TypeTuples::TTuple<OnlyType>> {
        using Ret = ErrorReturn;
    };

    template <typename OnlyType>
    struct GetFromTTupleWithErrorReturnImpl<0, TypeTuples::TTuple<OnlyType>> {
        using Ret = OnlyType;
    };
}

#endif //TASK2_GET_FROM_TTUPLE_HPP

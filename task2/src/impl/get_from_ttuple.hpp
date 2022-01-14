//
// Created by gogagum on 13.01.2022.
//

#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TASK2_GET_FROM_TTUPLE_HPP
#define TASK2_GET_FROM_TTUPLE_HPP

#include "../type_lists_concepts.hpp"
#include "../../type_tuples.hpp"
#include "error_return.hpp"

namespace TypeLists::Impl {
    /**
     * GetFromTTuple implementation.
     * @tparam N - number of elements which are to skip.
     * @tparam TTupleT - TTuple with list of elements.
     */
    template <std::size_t N, typename TTupleT>
    struct GetFromTTupleImpl;

    template <std::size_t N, typename FirstType, typename... OtherTypes>
    struct GetFromTTupleImpl<N, TypeTuples::TTuple<FirstType, OtherTypes...>> {
      private:
        using OthersTypeTTuple = TypeTuples::TTuple<OtherTypes...>;
      public:
        using Ret = typename GetFromTTupleImpl<N - 1, OthersTypeTTuple>::Ret;
    };

    template <typename FirstType, typename... OtherTypes>
    struct GetFromTTupleImpl<0, TypeTuples::TTuple<FirstType, OtherTypes...>> {
        using Ret = FirstType;
    };

    template <typename OnlyType>
    struct GetFromTTupleImpl<0, TypeTuples::TTuple<OnlyType>> {
        using Ret = OnlyType;
    };

    /**
     * GetFromTTupleWithErrorReturn implementation.
     * @tparam N - number of elements which are to skip.
     * @tparam TTupleT - TTuple with list of elements.
     */
    template <std::size_t N, typename TTupleT>
    struct GetFromTTupleWithErrorReturnImpl;

    template <std::size_t N, typename FirstType, typename... OtherTypes>
    struct GetFromTTupleWithErrorReturnImpl<N, TypeTuples::TTuple<FirstType, OtherTypes...>> {
      private:
        using OthersTypeTTuple = TypeTuples::TTuple<OtherTypes...>;
      public:
        using Ret =
            typename GetFromTTupleWithErrorReturnImpl<N - 1, OthersTypeTTuple>::Ret;
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

//
// Created by gogagum on 13.01.2022.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef TASK2_TYPE_TUPLES_HPP
#define TASK2_TYPE_TUPLES_HPP

namespace TypeTuples {
    /*
    * struct TTuple
    */
    template<class...>
    struct TTuple {};

    /*
     * TTupleCons
     */
    template <typename Added, class TypeTuple>
    struct TTupleCons;

    template <typename Added, typename... TTupleTypes>
    struct TTupleCons<Added, TypeTuples::TTuple<TTupleTypes...>> {
        using Ret = TypeTuples::TTuple<Added, TTupleTypes...>;
    };
}

#endif //TASK2_TYPE_TUPLES_HPP

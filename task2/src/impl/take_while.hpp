//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_TAKE_WHILE_HPP
#define TASK2_TAKE_WHILE_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    template<template<typename> class P, TypeList TL>
    struct TakeWhileImpl {
        using Ret = Nil;
    };

    template<template<typename> class P, TypeSequence TS>
    struct TakeWhileImpl<P, TS> {
    private:
        template<bool takeHead, typename _>
        struct Decision {
            using Ret = Nil;
        };

        template<typename _>
        struct Decision<true, _> {
            struct Ret {
                using Head = typename TS::Head;
                using Tail = typename TakeWhileImpl<P, typename TS::Tail>::Ret;
            };
        };

        template<typename _>
        struct Decision<false, _>{
            using Ret = Nil;
        };
    public:
        using Ret = typename Decision<P<typename TS::Head>::Value, Nil>::Ret;
    };
}

#endif //TASK2_TAKE_WHILE_HPP

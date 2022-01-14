//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_ZIP_TO_LIST_HPP
#define TASK2_ZIP_TO_LIST_HPP

#include "../type_lists_concepts.hpp"
#include "../zip2_to_lists.hpp"
#include "../map.hpp"

namespace TypeLists::Impl {
    template <TypeSequence TS, TypeList ZippedOthers, TypeList... Others>
    struct Zip2ListImpl : Nil {};

    template <TypeSequence TS,
              TypeSequence ZippedOthers,
              TypeSequence FirstOfOthers,
              TypeList... Others>
    struct Zip2ListImpl<TS, ZippedOthers, FirstOfOthers, Others...> {
      public:
        using Head = Cons<typename TS::Head,
                          typename ZippedOthers::Head>;
        using Tail = Zip2ListImpl<typename TS::Tail,
                                  typename ZippedOthers::Tail,
                                  typename FirstOfOthers::Tail,
                                  typename Others::Tail...>;
    };

    template <TypeSequence TS,
            TypeSequence ZippedOthers,
            TypeList FirstOfOthers,
            TypeList SecondOfOthers>
    struct Zip2ListImpl<TS, ZippedOthers, FirstOfOthers, SecondOfOthers> {
      private:
        using ZippedOthersTails = Zip22Lists<typename FirstOfOthers::Tail, typename SecondOfOthers::Tail>;
      public:
        using Head = Cons<typename TS::Head, typename ZippedOthers::Head>;
        using Tail = Zip2ListImpl<TS,
                                  ZippedOthersTails,
                                  typename FirstOfOthers::Tail,
                                  typename SecondOfOthers::Tail>;
    };

    template <TypeSequence TS, TypeSequence ZippedOthers, TypeSequence OnlyOfOthers>
    struct Zip2ListImpl<TS, ZippedOthers, OnlyOfOthers> {
        using Head = Cons<typename TS::Head, typename ZippedOthers::Head>;

        template<typename T>
        using SingleElementList = Cons<T, Nil>;

        using OnlyOfOthersTailZip = Map<SingleElementList, typename OnlyOfOthers::Tail>;

        using Tail = Zip2ListImpl<typename TS::Tail,
                OnlyOfOthersTailZip,
                typename OnlyOfOthers::Tail>;
    };
}

#endif //TASK2_ZIP_TO_LIST_HPP

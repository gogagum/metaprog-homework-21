//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_SCANL_HPP
#define TASK2_SCANL_HPP

#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    template <template <class, class> class OP, class First, class Second, TypeList TL_>
    struct ScanlImpl : public Nil {};

    template <template <class, class> class OP, class First, class Second, TypeSequence TS_>
    struct ScanlImpl<OP, First, Second, TS_> {
        using Head = OP<First, Second>;
        using Tail = ScanlImpl<OP, Second, typename TS_::Head, typename TS_::Tail>;
    };

    template <template <class, class> class OP, class First, class Second, Empty TE>
    struct ScanlImpl<OP, First, Second, TE> {
        using Head = OP<First, Second>;
        using Tail = Nil;
    };
}

#endif //TASK2_SCANL_HPP

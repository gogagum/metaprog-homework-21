//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_CONVERT_TO_TYPE_TUPLE_HPP
#define TASK2_CONVERT_TO_TYPE_TUPLE_HPP

#include "../type_tuples.hpp"
#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl {
    /*
     * struct ConvertToTTupleImpl
     * Metafunction-helper for transformation TypeList into TTuple.
     */
    template <TypeList TL>
    struct ConvertToTypeTupleImpl {
        using Ret = TypeTuples::TTuple<>;
    };

    template <TypeSequence TS>
    struct ConvertToTypeTupleImpl<TS> {
    private:
        using OtherParams = typename ConvertToTypeTupleImpl<typename TS::Tail>::Ret;
        using FirstParam = typename TS::Head;
    public:
        using Ret = typename TypeTuples::TTupleCons<FirstParam , OtherParams>::Ret;
    };
}

#endif //TASK2_CONVERT_TO_TYPE_TUPLE_HPP

//
// Created by gogagum on 13.01.2021.
//

#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TYPE_LISTS_IMPL_HPP_
#define TYPE_LISTS_IMPL_HPP_

#include <cstddef>
#include "get_from_ttuple.hpp"
#include "../type_tuples.hpp"
#include "../type_lists_concepts.hpp"

namespace TypeLists::Impl{
/*
 * ApplyLogicNotToValue
 * --------------------
 * Value now is not value
 */
template <class P>
struct ApplyLogicNotToValue {
    const static bool Value = !P::Value;
};

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

template <typename Added, class TypeTuple>
struct TTupleConsImpl;

template <typename Added, typename... TTupleTypes>
struct TTupleConsImpl<Added, TypeTuples::TTuple<TTupleTypes...>> {
using Ret = TypeTuples::TTuple<Added, TTupleTypes...>;
};

/*
 * struct ConvertToTTupleImpl
 * Вспомогательная метафункция преобразования из TypeList в TTuple.
 * Возвращаемое значение в ConvertedTailImpl<TL>::Converted
 */
template <TypeList TL>
struct ConvertToTypeTupleImpl;

template <TypeSequence TS>
struct ConvertToTypeTupleImpl<TS> {
private:
    using OtherParams = typename ConvertToTypeTupleImpl<typename TS::Tail>::Ret;
    using FirstParam = typename TS::Head;
public:
    using Ret = typename TTupleConsImpl<FirstParam , OtherParams>::Ret;
};

template <Empty TE>
struct ConvertToTypeTupleImpl<TE> {
    using Ret = TypeTuples::TTuple<>;
};

template <template <typename> class P, TypeList TL>
struct SkipWhileImpl;

template <template <typename> class P, TypeSequence TS>
struct SkipWhileImpl<P, TS> {
private:
    template <bool Skip, typename _>
    struct Decision;

    template <typename _>
    struct Decision<true, _> {
        using Ret = TS;
    };

    template <typename _>
    struct Decision<false, _> {
        using Ret = typename SkipWhileImpl<P, typename TS::Tail>::Ret;
    };

public:
    using Ret = typename Decision<!P<typename TS::Head>::Value, TS>::Ret;
};

template <template <typename> class P, Empty TE>
struct SkipWhileImpl<P, TE> {
    using Ret = Nil;
};

template <template<typename> class P, TypeList TL>
struct FilterImpl{
    using Ret = Nil;
};

template <template<typename> class P, TypeSequence TS>
struct FilterImpl<P, TS> {
private:
    template <typename T>
    using NotP = ApplyLogicNotToValue<P<T>>;

    using Skipped = typename Impl::SkipWhileImpl<NotP, TS>::Ret;  // Skip all args for which P is false.

    template <TypeList TL_>
    struct AfterSkip;

    template <TypeSequence TS_>
    struct AfterSkip<TS_> {
        using Head = typename TS_::Head;
        using Tail = typename FilterImpl<P, typename TS_::Tail>::Ret;
    };

    template <Empty TE>
    struct AfterSkip<TE> : Nil {};
public:
    using Ret = AfterSkip<Skipped>;
};

template <std::size_t N, TypeList TL>
struct DropImpl;

template <TypeList TL>
struct DropImpl<0, TL> {
    using Ret = TL;
};

template <std::size_t N, Empty TE>
struct DropImpl<N, TE> {
    using Ret = Nil;
};

template <std::size_t N, TypeSequence TS>
struct DropImpl<N, TS> {
    using Ret = typename DropImpl<N - 1, typename TS::Tail>::Ret;
};


template <template <typename, typename> class OP, typename T, TypeList TL>
struct FoldlImpl {
    using Ret = T;
};

template <template<typename, typename> class OP, typename T, TypeSequence TS>
struct FoldlImpl<OP, T, TS> {
    using Ret =
    typename FoldlImpl<OP, OP<T, typename TS::Head>, typename TS::Tail>::Ret;
};


}  // namespace TypeLists::Impl


#endif // TYPE_LISTS_IMPL_HPP_

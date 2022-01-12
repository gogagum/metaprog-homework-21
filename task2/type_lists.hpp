//
// Created by gogagum on 30.10.2021.
//

#pragma ide diagnostic ignored "OCUnusedConceptInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TYPE_LISTS_HPP_
#define TYPE_LISTS_HPP_

#include <concepts>

namespace TypeTuples {
/*
* struct TTuple
*/
template<class...>
struct TTuple {};
}

namespace TypeLists {

////////////////////////////////////////////////////////////////////////////////
// TypeSequence - концепт из условия.
template<class TL>
concept TypeSequence =
    requires {
        typename TL::Head;
        typename TL::Tail;
    };

/*
 * Nil конец TypeList`а
 */
struct Nil {};

////////////////////////////////////////////////////////////////////////////////
// concept Empty - концепт из условия, пустой TypeList.
template<class TL>
concept Empty = std::derived_from<TL, Nil>;

////////////////////////////////////////////////////////////////////////////////
// concept TypeList - концепт из условия.
template<class TL>
concept TypeList = Empty<TL> || TypeSequence<TL>;

struct ErrorReturn {};

/*
 * ApplyLogicNotToValue
 * --------------------
 * Value now is not value
 */
    template <class P>
    struct ApplyLogicNotToValue {
        const static bool Value = !P::Value;
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Impl{

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

}

/*
 * GetFromTTuple<N, TT>
 * ------------------------
 * Get Nth element from TTuple TT.
 */
template <std::size_t N, typename TT>
using GetFromTTuple = typename Impl::GetFromTTupleImpl<N, TT>::Ret;

/*
 * GetFromTTupleWithErrorReturn<N, TTuple>
 * ---------------------------------------
 * Get Nth element from TTuple TT, or ErrorReturn, if TT is shorter than N.
 */
template <std::size_t N, typename TT>
using GetFromTTupleWithErrorReturn = typename Impl::GetFromTTupleWithErrorReturnImpl<N, TT>::Ret;

/*
 * GetFromTypeList<N, TL>
 * ----------------------
 * Get Nth element from TypeList TL.
 */
template <std::size_t N, TypeList TL>
using GetFromTypeList = typename Impl::GetFromTypeListImpl<N, TL>::Ret;

/*
 * GetFromTypeListWithErrorReturn<N, TL>
 * -------------------------------------
 * Get Nth element from TypeList TL, or ErrorReturn, if TL is shorter than N.
 */
template <std::size_t N, TypeList TL>
using GetFromTypeListWithErrorReturn = typename Impl::GetFromTypeListWithErrorReturnImpl<N, TL>::Ret;

/*
 * Concat
 */
template <TypeList TL1, TypeList TL2>
struct Concat : public Nil {};

template <TypeSequence TS, TypeList TL>
struct Concat<TS, TL> {
  using Head = typename TS::Head;
  using Tail = Concat<typename TS::Tail, TL>;
};

template <Empty TE, TypeSequence TS>
struct Concat<TE, TS> : public TS {};

/*
 * TTupleCons<Added, TT>
 * ---------------------
 * Concat two type tuples - Added and TT.
 */
template <typename Added, typename TT>
using TTupleCons = typename Impl::TTupleConsImpl<Added, TT>::Ret;


template <TypeList TL>
using ToTuple = typename Impl::ConvertToTypeTupleImpl<TL>::Ret;

/*
 * ConvertToTypeList
 */
template <class TTupleInput>
struct FromTuple;

template <class TakenType, class... Other>
struct FromTuple<TypeTuples::TTuple<TakenType, Other...>> {
  using Head = TakenType;
  using Tail = FromTuple<TypeTuples::TTuple<Other...>>;
};

template <class LastType>
struct FromTuple<TypeTuples::TTuple<LastType>> {
  using Head = LastType;
  using Tail = Nil;
};

template <>
struct FromTuple<TypeTuples::TTuple<>> : public Nil {};

/*
 * Cons<T, TL>
 * -----------
 * Add element T into beginning of TypeList TL.
 */
template <class T, TypeList TL>
struct Cons {
  using Head = T;
  using Tail = TL;
};

/*
 * Repeat<T>
 * ---------
 * Infinite TypeList of T.
 */
template <class T>
struct Repeat {
  using Head = T;
  using Tail = Repeat<T>;
};

/*
 * Take<N, TL>
 * -----------
 * Take first N types from TypeList TL
 */
template <std::size_t N, TypeList TL>
struct Take {
  using Head = typename TL::Head;
  using Tail = Take<N-1, typename TL::Tail>;
};

template <std::size_t N, Empty TE>
struct Take<N, TE> : public Nil {};

template <TypeList TL>
struct Take<0, TL> : public Nil {};

/*
 * concept LengthGreaterOrEqual
 */
template <typename TL, std::size_t len>
concept LengthGreaterOrEqual = TypeList<TL> && !std::is_same_v<GetFromTypeListWithErrorReturn<len - 1, TL>, ErrorReturn>;

/*
 * concept TypeListOfLength
 */
template <typename TL, std::size_t len>
concept TypeListOfLength = TypeList<TL> && LengthGreaterOrEqual<TL, len> && !LengthGreaterOrEqual<TL, len + 1>;

/*
 * Drop<N, TL>
 * -----------
 * Take all except for first N types from TypeList TL.
 */
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

template <std::size_t N, TypeList TL>
using Drop = typename DropImpl<N, TL>::Ret;

/*
 * Replicate<N, T>
 * ---------------
 * N times repeat type T in TypeList.
 */
template <std::size_t N, typename T>
struct Replicate {
    using Head = T;
    using Tail = Replicate<N-1, T>;
};

template <typename T>
struct Replicate<0, T> : Nil {};

/*
 * Map<F, TL>
 * ----------
 * Apply F to all types of TypeList TL.
 */
template <template<typename> class F, TypeList TL>
struct Map : public Nil {};

template <template<typename> class F, TypeSequence TS>
struct Map<F, TS> {
    using Head = F<typename TS::Head>;
    using Tail = Map<F, typename TS::Tail>;
};

/*
 * SkipWhile<P, TL>
 * ----------------
 * First TypeList tail for which P<TLTail::Head>::Value is true.
 */
template <template <typename> class P, TypeList TL>
using SkipWhile = typename Impl::SkipWhileImpl<P, TL>::Ret;

/*
 * Filter<P, TL>
 * -------------
 * Leave only types for which P<T>::Value is true.
 */
template <template <typename> class P, TypeList TL>
using Filter = typename Impl::FilterImpl<P, TL>::Ret;

/*
 * Iterate<F, T>
 * -------------
 * T, F<T>, F<F<T>>...
 */
template <template<typename> class F, class T>
struct Iterate {
  using Head = T;
  using Tail = Iterate<F, F<T>>;
};

/*
 * Cycle
 */
template <TypeList TL>
struct Cycle : Nil {};

template <TypeSequence TS>
struct Cycle<TS>{
 private:
  template <TypeList CurrTL>
  struct CycleImpl {
    using Head = typename TS::Head;
    using Tail = typename CycleImpl<TS>::Tail;
  };

  template <TypeSequence CurrTL>
  struct CycleImpl<CurrTL>{
    using Head = typename CurrTL::Head;
    using Tail = CycleImpl<typename CurrTL::Tail>;
  };

  using CycleRet = CycleImpl<TS>;

 public:

  using Head = typename CycleRet::Head;
  using Tail = typename CycleRet::Tail;
};
/*
 * Inits
 */
template <TypeList TL>
struct Inits {
  using Head = Nil;
  using Tail = Nil;
};

template <TypeSequence TS>
struct Inits<TS> {
 private:

  template <std::size_t N, TypeList Tail>
  struct InitsImpl;

  template <std::size_t N, Empty TE>
  struct InitsImpl<N, TE> {
      using Head = Take<N, TS>;
      using Tail = Nil;
  };

  template <std::size_t N, TypeSequence TS_>
  struct InitsImpl<N, TS_> {
      using Head = Take<N, TS>;
      using Tail = InitsImpl<N + 1, typename TS_::Tail>;
  };

  using InitsRet = InitsImpl<0, TS>;
 public:
  using Head = typename InitsRet::Head;
  using Tail = typename InitsRet::Tail;
};

/*
 * Tails
 */
template <TypeList TL>
struct Tails {
  using Head = Nil;
  using Tail = Nil;
};

template <TypeSequence TS>
struct Tails<TS> {
  using Head = TS;
  using Tail = Tails<typename TS::Tail>;
};

/*
 * Scanl
 */
template <template <class, class> class OP, typename T, TypeList TL>
struct Scanl : public Nil {};

template <template <class, class> class OP, typename T, TypeSequence TS>
struct Scanl<OP, T, TS> {
 private:
  template <class First, class Second, TypeList TL_>
  struct ScanlImpl;

  template <class First, class Second, TypeSequence TS_>
  struct ScanlImpl<First, Second, TS_> {
    using Head = OP<First, Second>;
    using Tail = ScanlImpl<Second, typename TS_::Head, typename TS_::Tail>;
  };

  template <class First, class Second, Empty TE>
  struct ScanlImpl<First, Second, TE> {
    using Head = OP<First, Second>;
    using Tail = Nil;
  };
  using ImplResult = ScanlImpl<T, T, TS>;
 public:
  using Head = typename ImplResult::Head;
  using Tail = typename ImplResult::Tail;
};


/*
 * Foldl
 */

template <template <typename, typename> class OP, typename T, TypeList TL>
struct FoldlImpl {
  using Ret = T;
};

template <template<typename, typename> class OP, typename T, TypeSequence TS>
struct FoldlImpl<OP, T, TS> {
  using Ret =
      typename FoldlImpl<OP, OP<T, typename TS::Head>, typename TS::Tail>::Ret;
};

template <template <class, class> class OP, typename T, TypeList TL>
using Foldl = typename FoldlImpl<OP, T, TL>::Ret;

/*
 * Zip22Lists, zip with every pair as TypeList
 */

template <TypeList TL1, TypeList TL2>
struct Zip22Lists : public Nil {};

template <TypeSequence TS1, TypeSequence TS2>
struct Zip22Lists<TS1, TS2> {
  using Head = FromTuple<TypeTuples::TTuple<typename TS1::Head, typename TS2::Head>>;
  using Tail = Zip22Lists<typename TS1::Tail, typename TS2::Tail>;
};

/*
 * Zip2
 */
template <TypeList TL1, TypeList TL2>
struct Zip2 : public Nil {};

template <TypeSequence TS1, TypeSequence TS2>
struct Zip2<TS1, TS2> {
  using Head = TypeTuples::TTuple<typename TS1::Head, typename TS2::Head>;
  using Tail = Zip2<typename TS1::Tail, typename TS2::Tail>;
};

/*
 * Zip2List
 */

template <TypeList... TLs>
struct Zip2List : Nil {};

template <TypeSequence TS, TypeList ZippedOthers, TypeList... Others>
struct Zip2ListImpl : Nil {};

template <TypeSequence TS, TypeSequence ZippedOthers, TypeList FirstOfOthers, TypeList... Others>
struct Zip2ListImpl<TS, ZippedOthers, FirstOfOthers, Others...> {
  using ZippedOthers_ = Zip2ListImpl<FirstOfOthers, Zip2List<Others...>, Others...>;

  using Head = Cons<typename TS::Head,
                    typename ZippedOthers_::Head>;
  using Tail = Zip2List<typename TS::Tail,
                   typename FirstOfOthers::Tail,
                   typename Others::Tail...>;
};

template <TypeSequence TS,
          TypeSequence ZippedOthers,
          TypeList FirstOfOthers,
          TypeList SecondOfOthers>
struct Zip2ListImpl<TS, ZippedOthers, FirstOfOthers, SecondOfOthers> {
  using ZippedOthers_ = Zip2ListImpl<FirstOfOthers, Zip2List<SecondOfOthers>, SecondOfOthers>;

  using Head = Cons<typename TS::Head,
                    typename ZippedOthers_::Head>;
  using Tail = Zip2List<typename TS::Tail,
                        typename FirstOfOthers::Tail,
                        typename SecondOfOthers::Tail>;
};

template <TypeSequence TS, TypeSequence ZippedOthers, TypeSequence OnlyOfOthers>
struct Zip2ListImpl<TS, ZippedOthers, OnlyOfOthers> {
  using Head = Cons<typename TS::Head,
                                     typename ZippedOthers::Head>;
  using Tail = Zip2List<typename TS::Tail, typename OnlyOfOthers::Tail>;
};

template <TypeSequence TS, TypeList... TLs>
struct Zip2List<TS, TLs...> : Zip2ListImpl<TS, Zip2List<TLs...>, TLs...>{};

template <TypeSequence TS>
struct Zip2List<TS>{
  using Head = Cons<typename TS::Head, Nil>;
  using Tail = Zip2List<typename TS::Tail>;
};

/*
 * Zip
 */

template <TypeList... TLs>
using Zip = Map<ToTuple, Zip2List<TLs...>>;

/*
 * GroupBy
 */

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
    using Ret = Cons<typename TS::Head, typename TakeWhileImpl<P, typename TS::Tail>::Ret>;
  };

  template<typename _>
  struct Decision<false, _>{
    using Ret = Nil;
  };
 public:
  using Ret = typename Decision<P<typename TS::Head>::Value, Nil>::Ret;
};

template <template<typename> class P, TypeList TL>
using TakeWhile = typename TakeWhileImpl<P, TL>::Ret;


template <template<class, class> class Eq, TypeList TL>
struct GroupBy : public Nil{};

template <template<class, class> class Eq, TypeSequence TS>
struct GroupBy<Eq, TS>{
 private:
  template<typename T>
  using EqualToHead = Eq<typename TS::Head, T>;
 public:
  using Head = TakeWhile<EqualToHead, TS>;
  using Tail = GroupBy<Eq, SkipWhile<EqualToHead, typename TS::Tail>>;
};

} // namespace TypeLists


template<auto V>
struct ValueTag{ static constexpr auto Value = V; };

template<class T, T... ts>
using VTuple = TypeTuples::TTuple<ValueTag<ts>...>;

template<typename T>
struct AddOneImpl;

template <auto value>
struct AddOneImpl<ValueTag<value>> {
  using Ret = ValueTag<value + 1>;
};

template <class VTag>
using AddOne = typename AddOneImpl<VTag>::Ret;

template <class F, class S>
struct SumImpl;

template <auto vF, auto vS>
struct SumImpl<ValueTag<vF>, ValueTag<vS>> {
  using Ret = ValueTag<vF + vS>;
};

template <class VTag1, class VTag2>
using Sum = typename SumImpl<VTag1, VTag2>::Ret;

template<typename F, typename S>
struct LastTwoAdder {
 private:
  using Sum_ = Sum<F, S>;
 public:
  using Head = Sum_;
  using Tail = LastTwoAdder<S, Sum_>;
};

using Fib = TypeLists::Cons<ValueTag<0>, TypeLists::Cons<ValueTag<1>, LastTwoAdder<ValueTag<0>, ValueTag<1>>>>;

template<TypeLists::TypeList TailNumbers>
struct PrimesImpl{
  template <typename VT>
  struct ToErase;

  template <auto num>
  struct ToErase<ValueTag<num>>{
    constexpr static bool Value = num % TailNumbers::Head::Value != 0;
  };

  using Head = typename TailNumbers::Head;
  using Tail = PrimesImpl<TypeLists::Filter<ToErase, typename TailNumbers::Tail>>;
};

template<TypeLists::Empty EmptyTail>
struct PrimesImpl<EmptyTail> : public TypeLists::Nil {};


using Nats = TypeLists::Iterate<AddOne, ValueTag<0>>;
using Primes = PrimesImpl<Nats::Tail::Tail>;

#endif //TYPE_LISTS_HPP_
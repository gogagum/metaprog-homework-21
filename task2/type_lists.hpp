//
// Created by gogagum on 30.10.2021.
//

#pragma ide diagnostic ignored "OCUnusedConceptInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"
#ifndef TYPE_LISTS_HPP_
#define TYPE_LISTS_HPP_

#include <concepts>
#include "src/impl/type_lists_impl.hpp"
#include "src/type_lists_concepts.hpp"
#include "type_tuples.hpp"
#include "src/inits.hpp"
#include "src/take.hpp"
#include "src/cycle.hpp"
#include "src/cons.hpp"
#include "src/zip2_to_lists.hpp"
#include "src/map.hpp"

namespace TypeLists {

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
 * TTupleCons<Added, TT>
 * ---------------------
 * Concat two type tuples - Added and TT.
 */
template <typename Added, typename TT>
using TTupleCons = typename TypeTuples::TTupleCons<Added, TT>::Ret;


template <TypeList TL>
using ToTuple = typename Impl::ConvertToTypeTupleImpl<TL>::Ret;

/*
 * ConvertToTypeList
 */
template <class TTupleInput>
struct FromTuple : public Nil {};

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
 * concept LengthGreaterOrEqual
 */
template <typename TL, std::size_t len>
concept LengthGreaterOrEqual = TypeList<TL> && !std::is_same_v<GetFromTypeListWithErrorReturn<len - 1, TL>, Impl::ErrorReturn>;

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
using Drop = typename Impl::DropImpl<N, TL>::Ret;

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
using Scanl = typename Impl::ScanlImpl<OP, T, T, TL>;

/*
 * Foldl
 */
template <template <class, class> class OP, typename T, TypeList TL>
using Foldl = typename Impl::FoldlImpl<OP, T, TL>::Ret;

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

template <TypeSequence TS, TypeList... TLs>
struct Zip2List<TS, TLs...> : Impl::Zip2ListImpl<TS, Zip2List<TLs...>, TLs...>{};

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
 * TakeWhile
 */
template <template<typename> class P, TypeList TL>
using TakeWhile = typename Impl::TakeWhileImpl<P, TL>::Ret;

/*
 * GroupBy
 */
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

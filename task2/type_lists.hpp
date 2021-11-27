//
// Created by gogagum on 30.10.2021.
//

#ifndef TYPE_LISTS_HPP_
#define TYPE_LISTS_HPP_

#include <concepts>

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

/*
 * struct TTuple
 */
template<class... Ts>
struct TTuple {};

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
    struct GetFromTTupleImpl<N, TTuple<FirstType, OtherTypes...>> {
        using Ret[[maybe_unused]] = typename GetFromTTupleImpl<N - 1, TTuple<OtherTypes...>>::Ret;
    };

    template <typename FirstType, typename... OtherTypes>
    struct GetFromTTupleImpl<0, TTuple<FirstType, OtherTypes...>> {
        using Ret[[maybe_unused]] = FirstType;
    };

    template <typename OnlyType>
    struct GetFromTTupleImpl<0, TTuple<OnlyType>> {
        using Ret[[maybe_unused]] = OnlyType;
    };

    /*
     * GetFromTTupleWithErrorReturnImpl<N, Types...>
     */
    template <std::size_t N, typename... Types>
    struct GetFromTTupleWithErrorReturnImpl;

    template <std::size_t N, typename FirstType, typename... OtherTypes>
    struct GetFromTTupleWithErrorReturnImpl<N, TTuple<FirstType, OtherTypes...>> {
        using Ret[[maybe_unused]] = typename GetFromTTupleWithErrorReturnImpl<N - 1, TTuple<OtherTypes...>>::Ret;
    };

    template <typename FirstType, typename... TTupleTypes>
    struct GetFromTTupleWithErrorReturnImpl<0, TTuple<FirstType, TTupleTypes...>> {
        using Ret[[maybe_unused]] = FirstType;
    };

    template <std::size_t N, typename OnlyType>
    struct GetFromTTupleWithErrorReturnImpl<N, TTuple<OnlyType>> {
        using Ret[[maybe_unused]] = ErrorReturn;
    };

    template <typename OnlyType>
    struct GetFromTTupleWithErrorReturnImpl<0, TTuple<OnlyType>> {
        using Ret[[maybe_unused]] = OnlyType;
    };

    template <std::size_t N, TypeList TL>
    struct GetFromTypeListImpl;

    template <std::size_t N, TypeSequence TS>
    struct GetFromTypeListImpl<N, TS> {
        using Ret[[maybe_unused]] = typename GetFromTypeListImpl<N - 1, typename TS::Tail>::Ret;
    };

    template <TypeSequence TS>
    struct GetFromTypeListImpl<0, TS> {
        using Ret[[maybe_unused]] = typename TS::Head;
    };

    template <std::size_t N, TypeList TL>
    struct GetFromTypeListWithErrorReturnImpl;

    template <std::size_t N, TypeSequence TS>
    struct GetFromTypeListWithErrorReturnImpl<N, TS> {
        using Ret[[maybe_unused]] = typename GetFromTypeListWithErrorReturnImpl<N - 1, typename TS::Tail>::Ret;
    };

    template <TypeSequence TS>
    struct GetFromTypeListWithErrorReturnImpl<0, TS> {
        using Ret[[maybe_unused]] = typename TS::Head;
    };

    template <std::size_t N, Empty TE>
    struct GetFromTypeListWithErrorReturnImpl<N, TE> {
        using Ret[[maybe_unused]] = ErrorReturn;
    };

    template <typename Added, class TypeTuple>
    struct TTupleConsImpl;

    template <typename Added, typename... TTupleTypes>
    struct TTupleConsImpl<Added, TTuple<TTupleTypes...>> {
        using Ret[[maybe_unused]] = TTuple<Added, TTupleTypes...>;
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
        using Ret[[maybe_unused]] = typename TTupleConsImpl<FirstParam , OtherParams>::Ret;
    };

    template <Empty TE>
    struct ConvertToTypeTupleImpl<TE> {
        using Ret[[maybe_unused]] = TTuple<>;
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

    template <template <typename Arg> class P, TypeList TL>
    struct FilterImpl{
        using Ret = Nil;
    };

    template <template <typename> class P, TypeSequence TS>
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
template <size_t N, TypeList TL>
using GetFromTypeList = typename Impl::GetFromTypeListImpl<N, TL>::Ret;

/*
 * GetFromTypeListWithErrorReturn<N, TL>
 * -------------------------------------
 * Get Nth element from TypeList TL, or ErrorReturn, if TL is shorter than N.
 */
template <size_t N, TypeList TL>
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
using ConvertToTTuple = typename Impl::ConvertToTypeTupleImpl<TL>::Ret;

/*
 * ConvertToTypeList
 */
template <class TTupleInput>
struct ConvertToTypeList {
 private:

  /*
   * ConvertToTypeListImpl - вспомогательная метафункция от списка типов.
   */
  template <typename TakenType, typename... Other>
  struct ConvertToTypeListImpl {
    using Head = TakenType;
    using Tail = ConvertToTypeListImpl<Other...>;
  };

  /*
   * ConvertToTypeListImpl - случай последнего элемента в списке типов
   */
  template <typename Last>
  struct ConvertToTypeListImpl<Last> {
    using Head = Last;
    using Tail = Nil;
  };

  // getHeadImpl(TTupleInput<Types...>) - вспомогательная, нигде не вызываемая функция.
  // Схема работы:
  // TTuple<class... Types> -> class... Types -> ConvertToTypeListImpl<Types...>
  // -> TypeList::Head
  template <typename... Types>
  static typename ConvertToTypeListImpl<Types...>::Head
  getHeadImpl(TTuple<Types...>) {
      return {};
  }

  // getHeadImpl(TTupleInput<Types...>) - вспомогательная, нигде не вызываемая
  // функция.
  // Схема работы:
  // TTuple<class... Types> -> class... Types -> ConvertToTypeListImpl<Types...>
  // -> TypeList::Tail
  template <typename... Types>
  static typename ConvertToTypeListImpl<Types...>::Tail
  getTailImpl(TTuple<Types...>) {
      return {};
  }

 public:
  using Head [[maybe_unused]] = decltype(getHeadImpl(std::declval<TTupleInput>()));
  using Tail [[maybe_unused]] = decltype(getTailImpl(std::declval<TTupleInput>()));
};

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

template <TypeList TL>
struct Take<0, TL> : Nil {};

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
struct Drop {
  using Head [[maybe_unused]] = typename Drop<N-1, typename TL::Tail>::Head;
  using Tail = typename Drop<N-1, typename TL::Tail>::Tail;
};

template <TypeSequence TS>
struct Drop<0, TS> {
  using Head [[maybe_unused]] = typename TS::Head;
  using Tail = typename TS::Tail;
};

template <Empty TE>
struct Drop<0, TE> : public Nil {};

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
template <template<typename T> class F, TypeList TL>
struct Map : public Nil {};

template <template<typename T> class F, TypeSequence TS>
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
template <template <typename Arg> class F, class T>
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
    using Tail [[maybe_unused]] = typename CycleImpl<TS>::Tail;
  };

  template <TypeSequence CurrTL>
  struct CycleImpl<CurrTL>{
    using Head = typename CurrTL::Head;
    using Tail [[maybe_unused]] = CycleImpl<typename CurrTL::Tail>;
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
struct Inits;

template <TypeSequence TS>
struct Inits<TS> {
 private:
  template <std::size_t N, TypeList Tail_>
  struct InitsImpl{
    using Head = Take<N, TS>;
    using Tail = Nil;
  };

  template <std::size_t N, TypeSequence Tail_>
  struct InitsImpl<N, Tail_> {
    using Head = Take<N, TS>;
    using Tail = InitsImpl<N + 1, typename Tail_::Tail>;
  };

  using InitsRet = InitsImpl<1, typename TS::Tail>;
 public:
  using Head = typename InitsRet::Head;
  using Tail = typename InitsRet::Tail;
};

/*
 * Tails
 */
template <TypeList TL>
struct Tails : public Nil {};

template <TypeSequence TS>
struct Tails<TS> {
  using Head = typename TS::Tail;
  using Tail = Tails<typename TS::Tail>;
};

/*
 * Scanl
 */
template <template <class Arg1, class Arg2> class OP, typename T, TypeList TL>
struct Scanl : public Nil {};

template <template <class Arg1, class Arg2> class OP, typename T, TypeSequence TS>
struct Scanl<OP, T, TS> {
  using Head = typename OP<T, typename TS::Head>::Type;
  using Tail = Scanl<OP, Head, typename TS::Tail>;
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

template <template <class Arg1, class Arg2> class OP, typename T, TypeList TL>
using Foldl = typename FoldlImpl<OP, T, TL>::Ret;

/*
 * Zip2
 */

template <TypeList TL1, TypeList TL2>
struct Zip2 : public Nil {};

template <TypeSequence TS1, TypeSequence TS2>
struct Zip2<TS1, TS2> {
  using Head = ConvertToTypeList<TTuple<typename TS1::Head, typename TS2::Head>>;
  using Tail = Zip2<typename TS1::Tail, typename TS2::Tail>;
};

/*
 * Zip
 */

template <TypeList... TLs>
struct Zip : Nil {};

template <TypeSequence TS, TypeList ZippedOthers, TypeList... Others>
struct ZipImpl : Nil {};

template <TypeSequence TS, TypeSequence ZippedOthers, TypeList FirstOfOthers, TypeList... Others>
struct ZipImpl<TS, ZippedOthers, FirstOfOthers, Others...> {
  using ZippedOthers_ = ZipImpl<FirstOfOthers, Zip<Others...>, Others...>;

  using Head [[maybe_unused]] = Cons<typename TS::Head,
                    typename ZippedOthers_::Head>;
  using Tail [[maybe_unused]] = Zip<typename TS::Tail,
                   typename FirstOfOthers::Tail,
                   typename Others::Tail...>;
};

template <TypeSequence TS,
          TypeSequence ZippedOthers,
          TypeList FirstOfOthers,
          TypeList SecondOfOthers>
struct ZipImpl<TS, ZippedOthers, FirstOfOthers, SecondOfOthers> {
  using ZippedOthers_ = ZipImpl<FirstOfOthers, Zip<SecondOfOthers>, SecondOfOthers>;

  using Head = Cons<typename TS::Head,
                    typename ZippedOthers_::Head>;
  using Tail = Zip<typename TS::Tail,
                   typename FirstOfOthers::Tail,
                   typename SecondOfOthers::Tail>;
};

template <TypeSequence TS, TypeSequence ZippedOthers, TypeSequence OnlyOfOthers>
struct ZipImpl<TS, ZippedOthers, OnlyOfOthers> {
  using Head [[maybe_unused]] = Cons<typename TS::Head,
                                     typename ZippedOthers::Head>;
  using Tail [[maybe_unused]] = Zip<typename TS::Tail,
                                    typename OnlyOfOthers::Tail>;
};

template <TypeSequence TS, TypeList... TLs>
struct Zip<TS, TLs...> : ZipImpl<TS, Zip<TLs...>, TLs...>{};

template <TypeSequence TS>
struct Zip<TS>{
  using Head [[maybe_unused]] = Cons<typename TS::Head, Nil>;
  using Tail [[maybe_unused]] = Zip<typename TS::Tail>;
};

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

#endif //TYPE_LISTS_HPP_

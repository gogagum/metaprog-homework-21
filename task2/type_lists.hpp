//
// Created by gogagum on 30.10.2021.
//

#ifndef TYPE_LISTS_HPP_
#define TYPE_LISTS_HPP_

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
 * struct TypeListStruct - реализация шаблона - потенциально бесконечного
 * списка типов
 */
template <class T,  TypeList TL>
struct TypeListStruct {
  using Head [[maybe_unused]] = T;
  using Tail [[maybe_unused]] = TL;
};

/*
 * struct TTuple
 */
template<class... Ts>
struct TTuple {};

template <class T>
class ConcatHelper {
  ConcatHelper() = default;

 public:
  template <TypeList TL>
  static TypeListStruct<T, TL> prependReturn(TL) {
      return {};
  }
};

/*
 * struct ConvertToTTupleImpl
 * Вспомогательная метафункция преобразования из TypeList в TTuple.
 * Возвращаемое значение в ConvertedTailImpl<TL>::Converted
 */
template <TypeList TL>
struct ConvertToTTupleImpl;

template <Empty TE>
struct ConvertToTTupleImpl<TE>{
  using Converted [[maybe_unused]] = TTuple<>;
};

template <TypeSequence TS>
struct ConvertToTTupleImpl<TS>{
 private:
  using ConvertedTail = ConvertToTTupleImpl<typename TS::Tail>;
  /*
   * Вспомогательная функция преобразования из TypeList в TTuple.
   */
  template<class... TailClasses>
  [[maybe_unused]] static TTuple<typename TS::Head, TailClasses...>
  convertImpl(TTuple<TailClasses...>)  {
      return {};
  }

  [[maybe_unused]] static TTuple<typename TS::Head>
  convertImpl(TTuple<>) {
      return {};
  }

 public:
  /*
   * Преобразование к TTuple
   */
  using Converted [[maybe_unused]] =
      decltype(convertImpl(std::declval<typename ConvertedTail::Converted>()));
};

/*
 * ConvertToTTuple - метафункция преобразования TypeList к TTuple.
 */
template <TypeList TL>
using ConvertToTTuple = typename ConvertToTTupleImpl<TL>::Converted;

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
  struct ConvertToTypeListImpl{
    using Head = TakenType;
    using Tail = ConvertToTypeListImpl<Other...>;
  };

  /*
   * ConvertToTypeListImpl - случай последнего элемента в списке типов
   */
  template <typename Last>
  struct ConvertToTypeListImpl<Last>{
    using Head = Last;
    using Tail = Nil;
  };

  // getHeadImpl(TTupleInput<Types...>) - вспомогательная, нигде не вызываемая
  // функция.
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

  using Head = decltype(getHeadImpl(std::declval<TTupleInput>()));
  using Tail = decltype(getTailImpl(std::declval<TTupleInput>()));
};

////////////////////////////////////////////////////////////////////////////////
// Cons - добавление элемента в бесконечный списокty
template <class T, TypeList TL>
using Cons =
    decltype(
        ConcatHelper<T>::prependReturn(std::declval<TL>())
    );

////////////////////////////////////////////////////////////////////////////////
// Repeat - бесконечное повторение типа
template <class T>
struct Repeat {
  using Head = T;
  using Tail = Repeat<T>;
};


////////////////////////////////////////////////////////////////////////////////
// Take - взять первые N элементов из TypeList TL
template <size_t N, TypeList TL>
struct Take {
  using Head = typename TL::Head;
  using Tail = Take<N-1, typename TL::Tail>;
};

template <TypeList TL>
struct Take<1, TL> {
  using Head = typename TL::Head;
  using Tail = Nil;
};

////////////////////////////////////////////////////////////////////////////////
// Drop - взять всё, кроме первых N элементов из TL
template <size_t N, TypeList TL>
struct Drop {
 private:
  using DroppedTail = Drop<N-1, typename TL::Tail>;
 public:
  using Head = typename DroppedTail::Head;
  using Tail = typename DroppedTail::Tail;
};

template <TypeList TL>
struct Drop<0, TL> {
  using Head = typename TL::Head;
  using Tail = typename TL::Tail;
};

////////////////////////////////////////////////////////////////////////////////
// Replicate - N раз повторять тип T
template <size_t N, typename T>
struct Replicate {
  using Head = T;
  using Tail = Replicate<N-1, T>;
};

template <typename T>
struct Replicate<1, T>{
  using Head = T;
  using Tail = Nil;
};

////////////////////////////////////////////////////////////////////////////////
// Map список из результатов применения F к элементам TL
template <template<typename T> class F, TypeList TS>
struct Map {
 private:
  template <TypeSequence Tail>
  static Map<F, Tail> MappedTail(Tail) {
      return {};
  }

  template <Empty Tail>
  static Nil mappedTail(Tail) {
      return {};
  }

 public:
  using Head = F<typename TS::Head>;
  using Tail = decltype(MappedTail(std::declval<TS>()));
};

////////////////////////////////////////////////////////////////////////////////
// Filter

template <template <typename Arg> class P, TypeList TL>
struct Filter;

template <template <typename Arg> class P, TypeSequence TS>
struct Filter<P, TS>;

template <template <typename Arg> class P, Empty TE>
struct Filter<P, TE>;

template <template <typename Arg> class P, class TLHead, TypeList TLTail>
struct FilterImpl;

template <template <typename Arg> class P, class TLHead, TypeSequence TLTail>
struct FilterImpl<P, TLHead, TLTail> {
  template <bool takeFirst>
  struct HeadDecision;

  template<>
  struct HeadDecision<true> {
    using Head = TLHead;
    using Tail = Filter<P, TLTail>;
  };

  template<>
  struct HeadDecision<false> : public Filter<P, TLTail>{};

  using Ret = HeadDecision<P<TLHead>::Value>;
};

template <template <typename Arg> class P, class TLHead, Empty EmptyTLTail>
struct FilterImpl<P, TLHead, EmptyTLTail> {
  template <bool takeFirst>
  struct HeadDecision;

  template<>
  struct HeadDecision<true> {
    using Head = TLHead;
    using Tail = Nil;
  };

  template<>
  struct HeadDecision<false> : Nil {};

  using Ret = HeadDecision<P<TLHead>::Value>;
};


template <template <typename Arg> class P, TypeSequence TS>
struct Filter<P, TS> : public FilterImpl<P, typename TS::Head, typename TS::Tail>::Ret {};

template <template <typename Arg> class P, Empty TE>
struct Filter<P, TE> : public Nil {};

/*
 * Iterate
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
struct Cycle;

template <TypeSequence TS>
struct Cycle<TS>{
 private:
  template <TypeList CurrTL>
  struct CycleImpl;

  template <TypeSequence CurrTL>
  struct CycleImpl<CurrTL>{
    using Head = typename CurrTL::Head;
    using Tail = CycleImpl<typename CurrTL::Tail>;
  };

  template <Empty EmptyTL>
  struct CycleImpl<EmptyTL> : CycleImpl<TS>{};

  using CycleRet = CycleImpl<TS>;

 public:

  using Head = typename CycleRet::Head;
  using Tail = typename CycleRet::Tail;
};

template <Empty TE>
struct Cycle<TE> : public Nil {};

/*
 * Inits
 */
template <TypeList TL>
struct Inits;

template <TypeSequence TS>
struct Inits<TS> {
 private:
  template <size_t N, TypeList Tail>
  struct InitsImpl;

  template <size_t N, TypeSequence Tail_>
  struct InitsImpl<N, Tail_> {
    using Head = Take<N, TS>;
    using Tail = InitsImpl<N + 1, typename Tail_::Tail>;
  };

  template <size_t N, Empty EmptyTail>
  struct InitsImpl<N, EmptyTail>{
      using Head = Take<N, TS>;
      using Tail = Nil;
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
struct Tails;

template <TypeSequence TS>
struct Tails<TS> {
  using Head = typename TS::Tail;
  using Tail = Tails<typename TS::Tail>;
};

template <Empty TE>
struct Tails<TE> : public Nil {};

/*
 * Scanl
 */
template <template <class Arg1, class Arg2> class OP, typename T, TypeList TL>
struct Scanl;

template <template <class Arg1, class Arg2> class OP, typename T, TypeSequence TS>
struct Scanl<OP, T, TS> {
  using Head = OP<T, typename TS::Head>;
  using Tail = Scanl<OP, typename TS::Head, typename TS::Tail>;
};

template <template <class Arg1, class Arg2> class OP, typename T, Empty TE>
struct Scanl<OP, T, TE> : public Nil {};

/*
 * Foldl
 */

template <template <class Arg1, class Arg2> class OP, typename T, TypeList TL>
struct FoldlImpl;

template <template<class Arg1, class Arg2> class OP, typename T, TypeSequence TS>
struct FoldlImpl<OP, T, TS> {
  using Ret = FoldlImpl<OP, OP<T, typename TS::Head>, typename TS::Tail>;
};

template <template<class Arg1, class Arg2> class OP, typename T, Empty TE>
struct FoldlImpl<OP, T, TE> {
  using Ret = T;
};

template <template <class Arg1, class Arg2> class OP, typename T, TypeList TL>
using Foldl = typename FoldlImpl<OP, T, TL>::Ret;

/*
 * Zip2
 */

template <TypeList TL1, TypeList TL2>
struct Zip2;

template <TypeSequence TS1, TypeSequence TS2>
struct Zip2<TS1, TS2> {
  using Head = ConvertToTypeList<TTuple<typename TS1::Head, typename TS2::Head>>;
  using Tail = Zip2<typename TS1::Tail, typename TS2::Tail>;
};

template <TypeSequence TS, Empty TE>
struct Zip2<TS, TE> : public Nil {};

template <Empty TE, TypeSequence TS>
struct Zip2<TE, TS> : public Nil {};

/*
 * Zip
 */

template <TypeList... TLs>
struct ZipImpl;

template <TypeSequence TS, TypeList... TLs>
struct ZipImpl<TS, TLs...>{
  using Ret = Zip2<TS, typename ZipImpl<TLs...>::Ret>;
};

template <TypeSequence TS>
struct ZipImpl<TS>{
  using Ret = TS;
};

template <TypeList... TLs>
using Zip = ZipImpl<TLs...>;

/*
 * GroupBy
 */




} // namespace TypeLists

#endif //TYPE_LISTS_HPP_

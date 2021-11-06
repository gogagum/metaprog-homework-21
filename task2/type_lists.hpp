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

template <>
struct ConvertToTTupleImpl<Nil>{
  using Converted = TTuple<>;
};

template <TypeList TL>
struct ConvertToTTupleImpl{
 private:
  using ConvertedTail = ConvertToTTupleImpl<typename TL::Tail>;
  /*
   * Вспомогательная функция преобразования из TypeList в TTuple.
   */
  template<class... TailClasses>
  static TTuple<typename TL::Head, TailClasses...>
  convertImpl(TTuple<TailClasses...>)  {
      return {};
  }

  static TTuple<typename TL::Head>
  convertImpl(TTuple<>) {
      return {};
  }

 public:
  /*
   * Преобразование к TTuple
   */
  using Converted = decltype(convertImpl(std::declval<typename ConvertedTail::Converted>()));
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
template <class P, TypeList TL>
struct Filter {
  template <bool takeHead>
  static auto filterImpl();

  template <>
  static TypeListStruct<typename TL::Head, Filter<P, typename TL::Tail>>
  filterImpl<true>() {
      return {};
  }

  template  <>
  static Filter<P, typename TL::Tail> filterImpl<false>()  {
      return {};
  }

  using Ret =
      decltype(filterImpl(std::declval<P::template Value<typename TL::Head>>()));
  using Head = typename Ret::Head;
  using Tail = typename Ret::Tail;
};








} // namespace TypeLists

#endif //TYPE_LISTS_HPP_

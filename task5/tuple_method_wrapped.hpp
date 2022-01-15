//
// Created by gogagum on 20.12.2021.
//

#ifndef TASK5__TO_TYPE_LIST_HPP_
#define TASK5__TO_TYPE_LIST_HPP_

#include "type_lisis.hpp"
#include "impl/to_tuple_impl.hpp"
#include "impl/elements_count_impl.hpp"
#include "annotations_checker.hpp"

template <class T>
struct GetAllFields {
 private:
  constexpr static std::size_t allFieldsCount = countAllFieldsImpl<T, 0>();

  using STDTuple = decltype(toTupleImpl<T, allFieldsCount>(T{}));

  template <class T_>
  struct STDTupleToTTuple;

  template<class... AllFieldsTypes>
  struct STDTupleToTTuple<std::tuple<AllFieldsTypes...>> {
    using Ret = TypeLists::TTuple<AllFieldsTypes...>;
  };

  using FieldsConstRefs = typename STDTupleToTTuple<STDTuple>::Ret;
  using FieldsConstRefsSequence = TypeLists::ConvertToTypeList<FieldsConstRefs>;

  using FieldsSequence = TypeLists::Map<std::remove_cvref_t, FieldsConstRefsSequence>;

 public:
  using Ret = TypeLists::ConvertToTTuple<FieldsSequence>;
};

template <class T>
struct GetFields {
 private:
  using AllFields = typename GetAllFields<T>::Ret;
 public:
  using Ret = TypeLists::FilterTTuple<IsNotAnnotation, AllFields>;
};


#endif //TASK5__TO_TYPE_LIST_HPP_

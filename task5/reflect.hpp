//
// Created by gogagum on 18.12.2021.
//

#ifndef TASK5__REFLECT_HPP_
#define TASK5__REFLECT_HPP_

#include <cstddef>
#include <utility>
#include <array>
#include <tuple>
#include "impl/elements_count_impl.hpp"
#include "tuple_method_wrapped.hpp"
#include "impl/to_tuple_impl.hpp"
#include "annotations_farming.hpp"
#include "ubiq.hpp"

template <class T, class... Args>
concept AggregateConstructibleFrom = requires (Args... args) { T{args...}; };

template <class...>
class Annotate {};

template<class T, std::size_t numberOfArgs>
class CheckConstructibleFromUbiq{
 private:
  template <std::size_t... initNumbers>
  constexpr static bool checkConstructibleFromUbiqImpl(std::index_sequence<initNumbers...>) {
      return AggregateConstructibleFrom<T, UbiqConstructor<initNumbers>...>;
  };
 public:
  constexpr static bool ret = checkConstructibleFromUbiqImpl(std::make_index_sequence<numberOfArgs>());
};

template<class T, std::size_t pos>
struct NoAnnotationOnPos {
 private:
  template <std::size_t... initNumbers>
  constexpr static bool noAnnotateOnPosImpl(std::index_sequence<initNumbers...>) {
      return AggregateConstructibleFrom<T,
                                        UbiqConstructor<initNumbers>...,
                                        NoAnnotateUbiqConstructor>;
  };
 public:
  constexpr static bool ret =
      noAnnotateOnPosImpl(std::make_index_sequence<pos - 1>());
};

template<class T, std::size_t pos>
struct AnnotationOnPos {
  constexpr static bool ret =
      !NoAnnotationOnPos<T, pos>::ret;
};

////////////////////////////////////////////////////////////////////////////////
// AnnotationConcatWith<Annotation1>
template <class Annotation1>
struct AnnotationConcatWith;

////////////////////////////////////////////////////////////////////////////////
// Describe<T>
template <class T>
struct Describe {
 private:
  template <std::size_t num_fields_>
  requires (num_fields_ > 0)
  struct DescribeImpl {
    using Fields = typename GetFields<T>::Ret;
    using Annotations = typename GetAnnotations<T>::Ret;
  };
 public:
  static constexpr std::size_t num_fields = countFieldsImpl<T, 0, 0>();

  template<std::size_t I>
  struct DescriptorImpl {
    using Type = TypeLists::GetFromTTuple<I, typename DescribeImpl<num_fields>::Fields>;

    using Annotations = TypeLists::GetFromTTuple<I, typename DescribeImpl<num_fields>::Annotations>;

    template<class Flag>
    static constexpr bool has_annotation_class = AnnotationContains<Annotations, Flag>::Value;

    template<template<class...> class Flag>
    static constexpr bool has_annotation_template = AnnotationContainsTemplate<Annotations, Flag>::Value;

    template <template <class...> class AnnotationTemplate>
    using FindAnnotation = typename FindAnnotationTemplate<Annotations, AnnotationTemplate>::Ret;
  };

  template <std::size_t I>
  using Field = DescriptorImpl<I>;
};


#endif //TASK5__REFLECT_HPP_

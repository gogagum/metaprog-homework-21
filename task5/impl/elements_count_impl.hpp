//
// Created by gogagum on 20.12.2021.
//

#ifndef TASK5__ELEMENTS_COUNT_IMPL_HPP_
#define TASK5__ELEMENTS_COUNT_IMPL_HPP_

#include <cstddef>

template <typename T, std::size_t num>
struct CheckConstructibleFromUbiq;

template <typename T, std::size_t pos>
struct AnnotationOnPos;

template <typename T, std::size_t pos>
struct NoAnnotationOnPos;

////////////////////////////////////////////////////////////////////////////////
// countFieldsImpl() - случай Annotate в конце                                //
template <class T,
          std::size_t currLengthWithNoAnnotations,
          std::size_t currLengthWithAnnotations>
requires CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret &&
    AnnotationOnPos<T, currLengthWithAnnotations + 1>::ret
constexpr size_t countFieldsImpl();

////////////////////////////////////////////////////////////////////////////////
// countFieldsImpl() - случай не Annotate в конце                             //
template <class T,
          std::size_t currLengthWithNoAnnotations,
          std::size_t currLengthWithAnnotations>
requires CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret &&
         NoAnnotationOnPos<T, currLengthWithAnnotations + 1>::ret
constexpr size_t countFieldsImpl();

////////////////////////////////////////////////////////////////////////////////
// countFieldsImpl() - случай, когда уже всё                                  //
template <class T,
          std::size_t currLengthWithNoAnnotations,
          std::size_t currLengthWithAnnotations>
requires (!CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret)
constexpr std::size_t countFieldsImpl();

////////////////////////////////////////////////////////////////////////////////
// countAllFields() - ещё можно сконструироваться                             //
template <class T, std::size_t currLength>
requires CheckConstructibleFromUbiq<T, currLength + 1>::ret
constexpr std::size_t countAllFieldsImpl();

////////////////////////////////////////////////////////////////////////////////
// countAllFieldsImpl() - случай, когда уже всё
template <class T, std::size_t currLength>
requires (!CheckConstructibleFromUbiq<T, currLength + 1>::ret)
constexpr std::size_t countAllFieldsImpl();

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class T,
          std::size_t currLengthWithNoAnnotations,
          std::size_t currLengthWithAnnotations>
requires CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret &&
         AnnotationOnPos<T, currLengthWithAnnotations + 1>::ret
constexpr size_t countFieldsImpl() {
    return countFieldsImpl<T,
                           currLengthWithNoAnnotations,
                           currLengthWithAnnotations + 1>();
}

//----------------------------------------------------------------------------//
template <class T,
    std::size_t currLengthWithNoAnnotations,
    std::size_t currLengthWithAnnotations>
requires CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret &&
    NoAnnotationOnPos<T, currLengthWithAnnotations + 1>::ret
constexpr size_t countFieldsImpl() {
    // Последнее поле - аннотация
    return countFieldsImpl<T,
                           currLengthWithNoAnnotations + 1,
                           currLengthWithAnnotations + 1>();
}

//----------------------------------------------------------------------------//
template <class T,
    std::size_t currLengthWithNoAnnotations,
    std::size_t currLengthWithAnnotations>
requires (!CheckConstructibleFromUbiq<T, currLengthWithAnnotations + 1>::ret)
constexpr std::size_t countFieldsImpl() {
    // Возвращаемся
    return currLengthWithNoAnnotations;
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class T, std::size_t currLength>
requires CheckConstructibleFromUbiq<T, currLength + 1>::ret
constexpr std::size_t countAllFieldsImpl() {
    return countAllFieldsImpl<T, currLength + 1>();
}

//----------------------------------------------------------------------------//
template <class T, std::size_t currLength>
requires (!CheckConstructibleFromUbiq<T, currLength + 1>::ret)
constexpr std::size_t countAllFieldsImpl() {
    return currLength;
}

#endif //TASK5__ELEMENTS_COUNT_IMPL_HPP_

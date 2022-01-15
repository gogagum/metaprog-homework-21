//
// Created by gogagum on 20.12.2021.
//

#ifndef TASK5__UBIQ_HPP_
#define TASK5__UBIQ_HPP_

#include <cstddef>

template <class...>
class Annotate;

template <std::size_t>
struct UbiqConstructor {
  template <class Type>
  constexpr operator Type() const noexcept {
      return Type{};
  };
};

struct NoAnnotateUbiqConstructor {
  template <class Type>
  constexpr operator Type() const noexcept {
      return Type{};
  }

  template <class... Annotations>
  constexpr operator Annotate<Annotations...>() const noexcept = delete;
};

#endif //TASK5__UBIQ_HPP_

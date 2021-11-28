//
// Created by gogagum on 27.11.2021.
//

#ifndef TASK3__SPY_HPP_
#define TASK3__SPY_HPP_

#include "function.hpp"


template <class T>
class Spy {
 public:
  Spy() requires std::default_initializable<T>;

  Spy(const T& o) requires std::copyable<T>;

  Spy(T&& o) requires std::movable<T>;

  Spy(const Spy& other) requires std::copyable<T>;

  Spy(Spy&& other) noexcept;

  T& operator *();
  const T& operator *() const;

  T* operator ->();

  Spy& operator=(const Spy& other) requires std::copyable<T>;

  Spy& operator=(Spy&& other) noexcept requires std::movable<T>;

  bool operator==(const Spy& other) const requires std::equality_comparable<T>;

  /*
   * if needed (see below):
   *   equality operators
   *   destructor
  */

  template <std::invocable<unsigned int> Logger>
  requires std::copyable<Logger> && std::is_nothrow_destructible_v<Logger>
  void setLogger(const Logger& logger);

  template <std::invocable<unsigned int> Logger>
  requires (!std::copyable<T>) && std::is_nothrow_destructible_v<Logger>
  void setLogger(Logger&& logger);

  void setLogger(void(*)(unsigned int));

  ~Spy() noexcept requires std::is_nothrow_destructible_v<T> { };

  ~Spy() { };


 private:
  T held_;
  Function<void(unsigned int)> f_ = [](unsigned int){};
};


template <class T>
Spy<T>::Spy(const T& o) requires std::copyable<T> : held_(o) {}

template<class T>
Spy<T>::Spy(T &&o) requires std::movable<T> : held_(std::move(o)) {}

template<class T>
Spy<T>::Spy() requires std::default_initializable<T> = default;



template<class T>
Spy<T>::Spy(const Spy &other) requires std::copyable<T> : held_(other.held_), f_(other.f_) {}

template<class T>
Spy<T>::Spy(Spy &&other) noexcept : held_(std::move(other.held_)), f_(std::move(other.f_)) {}

template <class T>
T& Spy<T>::operator*() {
    return held_;
}

template <class T>
const T& Spy<T>::operator *() const {
    return held_;
}

template <class T>
T* Spy<T>::operator->() {
    f_(42); // TODO
    return &held_;
}
template<class T>
Spy<T>& Spy<T>::operator=(const Spy &other) requires std::copyable<T> {
    held_ = other.held_;
    f_ = other.f_;
    return *this;
}

template <class T>
template <std::invocable<unsigned int> Logger>
requires std::copyable<Logger> && std::is_nothrow_destructible_v<Logger>
void Spy<T>::setLogger(const Logger& logger) {
    f_ = logger;
}

template<class T>
template<std::invocable<unsigned int> Logger>
requires (!std::copyable<T>) && std::is_nothrow_destructible_v<Logger>
void Spy<T>::setLogger(Logger&& logger) {
    f_ = std::forward<Logger>(logger);
}

template<class T>
void Spy<T>::setLogger(void(*logger)(unsigned int)) {
    f_ = logger;
}

template<class T>
Spy<T>&
Spy<T>::operator=(Spy &&other) noexcept requires std::movable<T> {
    held_ = std::move(other.held_);
    f_ = std::move(other.f_);

    return *this;
}
template<class T>
bool Spy<T>::operator==(const Spy &other) const requires std::equality_comparable<T> {
    return held_ == other.held_;
}

#endif //TASK3__SPY_HPP_

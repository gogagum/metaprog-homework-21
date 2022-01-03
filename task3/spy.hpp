//
// Created by gogagum on 27.11.2021.
//

#ifndef TASK3__SPY_HPP_
#define TASK3__SPY_HPP_

#include "function.hpp"

template <class T>
concept MoveOnly = (!std::copyable<T>) && std::movable<T>;

template <class T>
class Spy {
public:
    class Proxy {
    public:
        // Constructor
        explicit Proxy(Spy<T>* owner) : owner_(owner) {};
        // Operator ->
        T* operator->() {
            return &owner_->held_;
        }

        ~Proxy() {
            assert(owner_->numberOfProxy_ != 0);
            if (owner_->numberOfProxy_ == 1) {
                owner_->f_(owner_->numberOfCalls_);
                owner_->numberOfCalls_ = 0;
            }
            owner_->numberOfProxy_--;
        };
    private:
        Spy<T>* owner_;
    };

public:
    Spy() requires std::default_initializable<T>;

    explicit Spy(const T& o) requires std::copyable<T>;

    explicit Spy(T&& o) noexcept requires std::movable<T>;

    Spy(const Spy& other) requires std::copyable<T>;

    [[maybe_unused]] Spy(Spy&& other) noexcept;

    T& operator *();
    const T& operator *() const;

    Proxy operator->();

    Spy& operator=(const Spy& other) requires std::copyable<T>;

    // Move - assigment
    Spy& operator=(Spy&& other) noexcept requires std::movable<T>;

    // Comparing
    bool operator==(const Spy& other) const requires std::equality_comparable<T>;

    template <std::invocable<unsigned int> Logger>
    requires std::copyable<Logger> && std::is_nothrow_destructible_v<Logger>
    void setLogger(const Logger& logger);

    template <std::invocable<unsigned int> Logger>
    requires (!std::copyable<T>)
             && MoveOnly<Logger>
             && std::is_nothrow_destructible_v<Logger>
    void setLogger(Logger&& logger) noexcept;;

private:
    std::size_t numberOfCalls_ = 0;
    std::size_t numberOfProxy_ = 0;
    T held_;
    Function<void(unsigned int)> f_;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class T>
Spy<T>::Spy(const T& o) requires std::copyable<T> : held_(o) {}

//----------------------------------------------------------------------------//
template<class T>
Spy<T>::Spy(T &&o) noexcept requires std::movable<T> : held_(std::move(o)) {}

//----------------------------------------------------------------------------//
template<class T>
Spy<T>::Spy() requires std::default_initializable<T> = default;

//----------------------------------------------------------------------------//
template<class T>
[[maybe_unused]] Spy<T>::Spy(const Spy &other) requires std::copyable<T>
        : held_(other.held_), f_(other.f_) {}

//----------------------------------------------------------------------------//
template<class T>
[[maybe_unused]] Spy<T>::Spy(Spy &&other) noexcept
        : held_(std::move(other.held_)), f_(other.f_) {}

//----------------------------------------------------------------------------//
template <class T>
T& Spy<T>::operator*() {
    return held_;
}

//----------------------------------------------------------------------------//
template <class T>
const T& Spy<T>::operator *() const {
    return held_;
}

//----------------------------------------------------------------------------//
template <class T>
typename Spy<T>::Proxy Spy<T>::operator->() {
    numberOfProxy_++;
    numberOfCalls_++;
    return Proxy(this);
}

//----------------------------------------------------------------------------//
template<class T>
Spy<T>& Spy<T>::operator=(const Spy &other) requires std::copyable<T> {
    held_ = other.held_;
    f_ = other.f_;
    return *this;
}

//----------------------------------------------------------------------------//
template <class T>
template <std::invocable<unsigned int> Logger>
requires std::copyable<Logger>&& std::is_nothrow_destructible_v<Logger>
void Spy<T>::setLogger(const Logger& logger) {
    f_ = logger;
}

//----------------------------------------------------------------------------//
template<class T>
Spy<T>&
Spy<T>::operator=(Spy<T> &&other) noexcept requires std::movable<T> {
    held_ = std::move(other.held_);
    f_ = std::forward<Function<void(unsigned)>>(other.f_);
    numberOfCalls_ = other.numberOfCalls_;
    numberOfProxy_ = other.numberOfProxy_;

    return *this;
}

//----------------------------------------------------------------------------//
template<class T>
bool Spy<T>::operator==(const Spy &other) const requires std::equality_comparable<T> {
    return held_ == other.held_;
}

//----------------------------------------------------------------------------//
template<class T>
template<std::invocable<unsigned int> Logger>
requires (!std::copyable<T>)
      && MoveOnly<Logger>
      && std::is_nothrow_destructible_v<Logger>
void Spy<T>::setLogger(Logger&& logger) noexcept {
    f_ = std::move(logger);
}


#endif //TASK3__SPY_HPP_
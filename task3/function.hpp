//
// Created by gogagum on 27.11.2021.
//

#ifndef TASK3__FUNCTION_HPP_
#define TASK3__FUNCTION_HPP_

#include <memory>
#include <functional>
#include <variant>
#include <cassert>

template<class F>
class Function;

template<class R, class... Args>
class Function<R(Args...)>
{
  constexpr static std::size_t SMALL_BUFFER_SIZE = 64;
  constexpr static std::size_t ALIGN = 16;

  using FuncPtr = R(*)(void*, Args...);

  struct InvokeTable {
    void(*dtor_)(void*) {nullptr};
    void*(*copy_)(void*) {nullptr};
  };

 public:

  Function();

  // & constructors
  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
      && std::copyable<F>
  explicit Function(const F& f);

  Function(const Function<R(Args...)>& other);

  // && constructors
  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
        && std::movable<F> && (!std::copyable<F>)
  explicit Function(F&& f) noexcept;

  Function(Function<R(Args...)>&& other) noexcept = default;

  // & assiqnment
  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>)
        && std::copyable<F>
  Function& operator=(const F& f);

  Function& operator=(const Function& other);
  //\ & assignment

  // && assignment
  Function& operator=(Function&& other) noexcept;

  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::movable<F>
  Function& operator=(F&& f) noexcept;
  //\ && assignment

  R operator()(Args... args);

  ~Function();

  void copyFromFunctionPointerImpl(R(*ptr)(Args...)) {
      data_ = (void*)ptr;
      call_ = +[](void* self, Args... args) -> R {
        return reinterpret_cast<R(*)(Args...)>(self)(args...);
      };
      table_.dtor_ = nullptr;
      table_.copy_ = +[](void* self) -> void* {
        return self;
      };
  }

  template<class F>
  void copyFromFunctionalObj(F* func) {
      data_ = new F(*static_cast<F*>(func));
      call_ = +[](void* self, Args...args) -> R {
          return static_cast<F*>(self)->operator()(args...);
      };
      table_.dtor_ = +[](void* self) {
          delete static_cast<F*>(self);
      };
      table_.copy_ = +[](void* self) -> void* {
          return new F(*static_cast<F*>(self));
      };
  }

 private:
  FuncPtr call_{nullptr};
  InvokeTable table_;
  void* data_;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
     && std::copyable<F>
Function<R(Args...)>::Function(const F& f) {
    using Func = std::remove_cvref_t<F>;
    if constexpr (std::is_same_v<Func, R(*)(Args...)>) {
        copyFromFunctionPointerImpl(f);
    } else {
        copyFromFunctionalObj(const_cast<Func*>(&f));
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::Function(const Function &other) {
    data_ = other.table_.copy_(other.data_);
    call_ = other.call_;
    table_.dtor_ = other.table_.dtor_;
    table_.copy_ = other.table_.copy_;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
      && std::movable<F> && (!std::copyable<F>)
Function<R(Args...)>::Function(F&& f) noexcept {
    using Func = std::remove_cvref_t<F>;

    data_ = new Func(std::forward<Func>(f));

    call_ =
        +[](void* self, Args... args) {
            return static_cast<Func*>(self)->operator()(args...);
        };
    table_.dtor_ =
        +[](void* self) {
            delete static_cast<Func*>(self);
        };
    table_.copy_ =
        +[](void* self) -> void* {
            return new Func(std::forward<Func>(*static_cast<F*>(self)));
        };
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(Function&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (table_.dtor_) {
        table_.dtor_(data_);
        table_.dtor_ = nullptr;
    }

    data_ = std::move(other.data_);
    call_ = std::move(other.call_);
    table_ = std::move(other.table_);

    other.table_.dtor_ = nullptr;

    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
       && std::copyable<F>
Function<R(Args...)>&
Function<R(Args...)>::operator=(const F& f) {
    using Func = std::remove_cvref_t<F>;

    if (table_.dtor_) {
        table_.dtor_(data_);
        table_.dtor_ = nullptr;
    }

    if constexpr (std::is_same_v<Func, R(*)(Args...)>) {
        copyFromFunctionPointerImpl(f);
    } else {
        copyFromFunctionalObj(const_cast<Func*>(&f));
    }

    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
      && std::movable<F>
Function<R(Args...)> &
Function<R(Args...)>::operator=(F&& f) noexcept {
    using Func = std::remove_cvref_t<F>;
    if (table_.dtor_) {
        table_.dtor_(data_);
        table_.dtor_ = nullptr;
    }

    if constexpr (std::is_same_v<Func, R(*)(Args...)>) {
        data_ = (void*)f;
        call_ = +[](void* self, Args... args) {
          return reinterpret_cast<Func>(self)(args...);
        };
        table_.dtor_ = nullptr;
        table_.copy_ = +[](void* self) -> void* {
          return self;
        };
    } else {
        data_ = new Func(std::forward<F>(f));
        call_ = +[](void* self, Args...args) {
          return static_cast<Func*>(self)->operator()(args...);
        };
        table_.dtor_ = +[](void* self) {
            delete static_cast<Func*>(self);
        };
        table_.copy_ = +[](void* self) -> void* {
            return new Func(std::forward<Func>(*static_cast<Func*>(self)));
        };
    }
    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
R Function<R(Args...)>::operator()(Args... args) {
    return call_(data_, args...);
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::~Function() {
    if (table_.dtor_ && data_ != nullptr) {
        table_.dtor_(data_);
        table_.dtor_ = nullptr;
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(const Function<R(Args...)>& other) {
    if (&other == this) {
        return *this;
    }

    if (table_.dtor_) {
        table_.dtor_(data_);
        table_.dtor_ = nullptr;
    }

    data_ = other.table_.copy_(other.data_);

    call_ = other.call_;
    table_.dtor_ = other.table_.dtor_;
    table_.copy_ = other.table_.copy_;

    return *this;
}
template<class R, class... Args>
Function<R(Args...)>::Function() {
    if constexpr(std::is_same_v<R, void>) {
        copyFromFunctionPointerImpl(+[](Args...) -> void { });
    } else {
        copyFromFunctionPointerImpl(+[](Args...) -> R { return R{};});
    }
}

#endif //TASK3__FUNCTION_HPP_

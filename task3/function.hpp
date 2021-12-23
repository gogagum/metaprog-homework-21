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
  using FuncPtr = R(*)(void*, Args...);

  struct InvokeTable
  {
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

  Function(Function<R(Args...)>&& other) noexcept ;

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

 private:
  void makeEmpty() {
      call_ = nullptr;
      table_.dtor_ = nullptr;
      table_.copy_ = nullptr;
      data_ = nullptr;

      isEmpty_ = true;
  }

 private:
  FuncPtr call_{nullptr};
  InvokeTable table_;
  void* data_;
  bool isEmpty_;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::Function() : isEmpty_(true) {
    makeEmpty();
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
    && std::copyable<F>
Function<R(Args...)>::Function(const F& f) : isEmpty_(false) {
    using Func = std::remove_cvref_t<F>;
    data_ = new Func(f);

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
          return new Func(*static_cast<Func*>(self));
        };
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::Function(const Function &other) : isEmpty_(false) {
    if (other.isEmpty_) {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        makeEmpty();
    } else {
        data_ = other.table_.copy_(other.data_);
        call_ = other.call_;
        table_.dtor_ = other.table_.dtor_;
        table_.copy_ = other.table_.copy_;
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
      && std::movable<F> && (!std::copyable<F>)
Function<R(Args...)>::Function(F&& f) noexcept : isEmpty_(false) {
    using Func = std::remove_cvref_t<F>;

    data_ = new Func(std::forward<Func>(f));

    call_ =
        +[](void* self, Args... args) {
          return static_cast<Func*>(self)->operator()(args...);
        };
    table_.dtor_ = +[](void* self) {
                       delete static_cast<Func*>(self);
                   };
    table_.copy_ =
        +[](void* self) -> void* {
          return new Func(*static_cast<F*>(self));
        };
}

//----------------------------------------------------------------------------//
template <class R, class... Args>
Function<R(Args...)>::Function(Function<R(Args...)>&& other) noexcept : isEmpty_(false) {
    if (other.isEmpty_) {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        makeEmpty();
    } else {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }

        data_ = other.table_.copy_(other.data_);
        call_ = std::move(other.call_);
        table_ = std::move(other.table_);

        other.table_.dtor_ = nullptr;
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(Function&& other) noexcept {
    if (other.isEmpty_) {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        makeEmpty();
    } else {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        isEmpty_ = false;

        data_ = other.data_;
        call_ = other.call_;
        table_ = other.table_;

        other.table_.dtor_ = nullptr;
    }

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

    isEmpty_ = false;
    if (table_.dtor_) {
    //    table_.dtor_(data_);
    }
    if constexpr (std::is_same_v<Func, void(*)(unsigned)>) {
        data_ = (void*)f;
        call_ = +[](void* self, Args... args) {
            return reinterpret_cast<Func>(self)(args...);
        };
        table_.dtor_ = nullptr;
        table_.copy_ = +[](void* self) -> void* {
                           return self;
                       };
    } else {
        data_ = (void*)&f;
        call_ = +[](void* self, Args...args) {
            return static_cast<Func*>(self)->operator()(args...);
        };
        table_.dtor_ = +[](void* self) { delete static_cast<Func*>(self); };
        table_.copy_ = +[](void* self) -> void* { return new Func(*static_cast<Func*>(self)); };
    }
    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<std::remove_cvref_t<F>, Function<R(Args...)>>)
      && std::movable<F>
Function<R(Args...)> &
Function<R(Args...)>::operator=(F &&f) noexcept {
    using Func = std::remove_cvref_t<F>;
    isEmpty_ = false;
    if (table_.dtor_) {
    //    table_.dtor_(data_);
    }

    if constexpr (std::is_same_v<Func, void(*)(unsigned)>) {
        data_ = (void*)f;
        call_ = +[](void* self, Args... args) {
          return reinterpret_cast<Func>(self)(args...);
        };
        table_.dtor_ = nullptr;
        table_.copy_ = +[](void* self) -> void* {
          return new Func(*static_cast<Func*>(self));
        };
    } else {
        data_ = (void*)&f;
        call_ = +[](void* self, Args...args) {
          return static_cast<Func*>(self)->operator()(args...);
        };
        table_.dtor_ = +[](void* self) { delete static_cast<Func*>(self); };
        table_.copy_ = +[](void* self) { return self; };
    }
    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
R Function<R(Args...)>::operator()(Args... args) {
    if (!isEmpty_) {
        return call_(data_, args...);
    }
    return R();
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::~Function() {
    if (table_.dtor_) {
    //    table_.dtor_(data_);
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(const Function<R(Args...)>& other) {
    if (&other == this) {
        return *this;
    }

    if (other.isEmpty_) {
        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        makeEmpty();
    } else {
        isEmpty_ = false;

        if (table_.dtor_) {
        //    table_.dtor_(data_);
        }
        data_ = other.table_.copy_(other.data_);

        call_ = other.call_;
        table_.dtor_ = other.table_.dtor_;
        table_.copy_ = other.table_.copy_;
    }

    return *this;
}


#endif //TASK3__FUNCTION_HPP_

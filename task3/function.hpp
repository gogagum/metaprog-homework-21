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

  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::movable<F>
  Function(F&& f) {
      using Func = std::remove_cvref_t<F>;
      data_ = new F(std::forward<F>(f));

      call_ =
          +[](void* self, Args... args) {
              return static_cast<F*>(self)->operator()(args...);
          };
      table_.dtor_ = +[](void* self) { delete static_cast<Func*>(self); };
      table_.copy_ =
          +[](void* self) -> void* {
              return new Func(*static_cast<Func*>(self));
          };
  }

  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::copyable<F>
  Function(const F& f) {
      using Func = std::remove_cvref_t<F>;
      data_ = new F(std::forward<F>(f));

      call_ =
          +[](void* self, Args... args) {
            return static_cast<F*>(self)->operator()(args...);
          };
      table_.dtor_ = +[](void* self) { delete static_cast<Func*>(self); };
      table_.copy_ =
          +[](void* self) -> void* {
            return new Func(*static_cast<Func*>(self));
          };
  }

  Function(const Function& other)
  {
      data_ = other.table_.copy_(other.data_);
      call_ = other.call_;
      table_.dtor_ = other.table_.dtor_;
      table_.copy_ = other.table_.copy_;
  }

  Function& operator=(const Function& other);
  Function& operator=(Function&& other) noexcept;

  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::copyable<F>
  Function& operator=(const F& f) {
      if (table_.dtor_) {
          table_.dtor_(data_);
      }
      data_ = (void*)&f;
      call_ =
          +[](void* self, Args... args) {
            return static_cast<F*>(self)->operator()(args...);
          };
      table_.dtor_ = +[](void*) {  };
      table_.copy_ = +[](void* self) { return self; };
      return *this;
  }

  template<class F>
  requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::movable<F>
  Function& operator=(F&& f) {
      if (table_.dtor_) {
          table_.dtor_(data_);
      }
      data_ = (void*)&f;
      call_ =
          +[](void* self, Args... args) {
            return static_cast<F*>(self)->operator()(args...);
          };
      table_.dtor_ = +[](void*) {  };
      table_.copy_ = +[](void* self) { return self; };
      return *this;
  }

  Function& operator=(R (*ptr)(Args...)) {
      if (table_.dtor_) {
          table_.dtor_(data_);
      }
      data_ = (void*)ptr;
      call_ =
          +[](void* self, Args... args) {
              return reinterpret_cast<R (*)(Args...)>(self)(args...);
          };
      table_.dtor_ =
          +[](void*) { /*???*/ };
      table_.copy_ = +[](void* self) { return self; };
      return *this;
  }

  R operator()(Args... args) {
      return call_(data_, args...);
  }

  ~Function() {
      if (table_.dtor_) {
          table_.dtor_(data_);
      }
  }

 private:
  FuncPtr call_{nullptr};
  InvokeTable table_;
  void* data_;
};

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(const Function<R(Args...)>& other) {
    assert(&other != this);

    if (table_.dtor_ != nullptr) {
        table_.dtor_(data_);
    }
    data_ = other.table_.copy_(other.data_);

    call_ = other.call_;
    table_.dtor_ = other.table_.dtor_;
    table_.copy_ = other.table_.copy_;

    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(Function&& other)  noexcept {
    if (table_.dtor_ != nullptr) {
        table_.dtor_(data_);
    }

    data_ = other.table_.copy_(other.data_);

    call_ = other.call_;
    table_.dtor_ = other.table_.dtor_;
    table_.copy_ = other.table_.copy_;

    return *this;
}

#endif //TASK3__FUNCTION_HPP_

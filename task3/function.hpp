//
// Created by gogagum on 27.11.2021.
//

#ifndef TASK3__FUNCTION_HPP_
#define TASK3__FUNCTION_HPP_

#include <functional>
#include <variant>
#include <cassert>

template<class F>
class Function;

template<class R, class... Args>
class Function<R(Args...)>
{
private:
    using FuncPtr = R(*)(Args...);

    using CallT = R(*)(void*, Args...);

    struct InvokeTable {
      using DestructorT = void(*)(void*);
      using CopyFuncT = void*(*)(void*);

      DestructorT dtor_ = nullptr;
      CopyFuncT copy_ = nullptr;
    };

public:
    /**
     * Default constructor.
     */
    Function();

    /**
     * Constructor from functional object.
     * @tparam F functional object type.
     * @param f functional object.
     */
    template<class F>
    explicit Function(const F& f);

    /**
     * Copy constructor from other function.
     * @param other function.
     */
    Function(const Function<R(Args...)>& other);

    /**
     * Move-constructor from movable functional object.
     * @tparam F functional object type.
     * @param f functional object.
     */
    template<class F>
    requires (!std::same_as<F, Function<R(Args...)>>) && std::movable<F>
    explicit Function(F&& f) noexcept;

    /**
     * Move constructor from other function.
     * @param other function.
     */
    Function(Function<R(Args...)>&& other) noexcept = default;

    /**                                                                         
     * Copy assignment.                                                         
     * @tparam F functional object type.                                        
     * @param f functional object.
     * @return reference to self.
     */
    template<class F>
    Function<R(Args...)>& operator=(const F& f);

    /**
     * Copy assignment from other function.
     * @param other function.
     * @return reference to self.
     */
    Function<R(Args...)>& operator=(const Function<R(Args...)>& other);

    /**
     * Move assignment from other function.
     * @param other function.
     * @return reference to self.
     */
    Function<R(Args...)>& operator=(Function<R(Args...)>&& other) noexcept;

    /**
     * Move assgnment from movable functional object.
     * @tparam F functional object type
     * @param f functional object.
     * @return reference to self.
     */
    template<class F>
    requires (!std::same_as<std::remove_cvref_t<F>, Function>) && std::movable<F>
    Function<R(Args...)>& operator=(F&& f) noexcept;

    /**
     * Call operator.
     * @param args arguments.
     * @return result of function.
     */
    R operator()(Args... args);

    /**
     * Destructor.
     */
    ~Function();

private:

    void copyFromFunctionPointerImpl(R(*ptr)(Args...));

    template<class F>
    void copyFromFunctionalObj(F* func);

    template<class Func>
    static R callByOperator(void* self, Args... args);

    static R callToFuncPtr(void* self, Args... args);

    template<class F>
    static void deleteImpl(void* self);

    template<class F>
    static void* copyWithNewAllocation(void* self);

    template<class F>
    static void* moveCopy(void* self);

    void callDtorImpl();

private:
    CallT call_{ nullptr };
    InvokeTable table_;
    void* data_;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
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
Function<R(Args...)>::Function(const Function<R(Args...)> &other) {
    data_ = other.table_.copy_(other.data_);
    call_ = other.call_;
    table_.dtor_ = other.table_.dtor_;
    table_.copy_ = other.table_.copy_;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
requires (!std::same_as<F, Function<R(Args...)>>) && std::movable<F>
Function<R(Args...)>::Function(F&& f) noexcept {
    using Func = std::remove_cvref_t<F>;

    data_ = new Func(std::forward<Func>(f));
    call_ = callByOperator<Func>;
    table_.dtor_ = deleteImpl<Func>;
    table_.copy_ = moveCopy<Func>;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(Function<R(Args...)>&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    callDtorImpl();
    data_ = std::move(other.data_);
    call_ = std::move(other.call_);
    table_ = std::move(other.table_);

    other.table_.dtor_ = nullptr;

    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
Function<R(Args...)>& Function<R(Args...)>::operator=(const F& f) {
    using Func = std::remove_cvref_t<F>;

    callDtorImpl();
    if constexpr (std::is_same_v<Func, FuncPtr>) {
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

    callDtorImpl();
    if constexpr (std::is_same_v<Func, FuncPtr>) {
        data_ = (void*)f;
        call_ = callToFuncPtr;
        table_.dtor_ = nullptr;
        table_.copy_ = +[](void* self) -> void* { return self; };
    } else {
        data_ = new Func(std::forward<F>(f));
        call_ = callByOperator<Func>;
        table_.dtor_ = deleteImpl<Func>;
        table_.copy_ = moveCopy<Func>;
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
    callDtorImpl();
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>&
Function<R(Args...)>::operator=(const Function<R(Args...)>& other) {
    if (&other == this) {
        return *this;
    }

    callDtorImpl();

    data_ = other.table_.copy_(other.data_);
    call_ = other.call_;
    table_ = other.table_;

    return *this;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
Function<R(Args...)>::Function() {
    if constexpr(std::is_same_v<R, void>) {
        copyFromFunctionPointerImpl(+[](Args...) -> void { });
    } else {
        copyFromFunctionPointerImpl(+[](Args...) -> R { return R{}; });
    }
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
void Function<R(Args...)>::copyFromFunctionPointerImpl(R (*ptr)(Args...)) {
    data_ = (void*)ptr;
    call_ = callToFuncPtr;
    table_.dtor_ = nullptr;
    table_.copy_ = +[](void* self) -> void* { return self; };
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
void Function<R(Args...)>::copyFromFunctionalObj(F *func) {
    data_ = new F(*static_cast<F*>(func));
    call_ = callByOperator<F>;
    table_.dtor_ = deleteImpl<F>;
    table_.copy_ = copyWithNewAllocation<F>;
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
R Function<R(Args...)>::callToFuncPtr(void *self, Args... args) {
    return reinterpret_cast<R(*)(Args...)>(self)(args...);
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class Func>
R Function<R(Args...)>::callByOperator(void *self, Args... args) {
    return static_cast<Func*>(self)->operator()(args...);
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
void Function<R(Args...)>::deleteImpl(void *self) {
    delete static_cast<F*>(self);
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
void *Function<R(Args...)>::copyWithNewAllocation(void *self) {
    return new F(*static_cast<F*>(self));
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
template<class F>
void *Function<R(Args...)>::moveCopy(void *self) {
    return new F(std::move(*static_cast<F*>(self)));
}

//----------------------------------------------------------------------------//
template<class R, class... Args>
void Function<R(Args...)>::callDtorImpl() {
    if (table_.dtor_) {
        table_.dtor_(data_);
    }
}

#endif //TASK3__FUNCTION_HPP_

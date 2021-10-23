#include <iostream>

#include "MappingTools.hpp"

struct Base {
  virtual ~Base() = default;
};

struct A : public Base {};
struct B : public Base {};

int main() {
    auto aA = A{};
    auto bB = B{};

    auto& a = static_cast<Base&>(aA);
    auto& b = static_cast<Base&>(bB);

    using TestClassMapper =
        ClassMapper<Base, int, Mapping<A, 1>, Mapping<B, 2>>;

    std::cout << TestClassMapper ::map(a).value() << std::endl;
    std::cout << TestClassMapper ::map(b).value() << std::endl;

    return 0;
}

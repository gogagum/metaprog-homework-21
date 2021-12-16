#include <iostream>
#include "enum_traits.hpp"

enum class Shape {
  SQUARE, CIRCLE = 5, LINE, POINT = -2
};

int main() {

    //ABC::abc(TestEnum(2));


    enum Fruit : unsigned short {
      APPLE, MELON
    };


    static_assert(EnumeratorTraits<Shape>::size() == 4);
    static_assert(EnumeratorTraits<Fruit>::size() == 2);

    static_assert(EnumeratorTraits<Fruit>::nameAt(0) == "APPLE");
    static_assert(EnumeratorTraits<Fruit>::nameAt(1) == "MELON");
    static_assert(EnumeratorTraits<Shape>::nameAt(0) == "POINT");

    static_assert(EnumeratorTraits<Shape>::at(0) == Shape::POINT);
    static_assert(EnumeratorTraits<Shape>::at(1) == Shape::SQUARE);
    static_assert(EnumeratorTraits<Shape>::at(3) == Shape::LINE);

    return 0;

}

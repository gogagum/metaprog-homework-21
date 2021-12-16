#include <iostream>
#include "enum_traits.hpp"

int main() {

    //ABC::abc(TestEnum(2));


    enum Fruit : unsigned short {
      APPLE, MELON
    };

    enum class Shape {
      SQUARE, CIRCLE = 5, LINE, POINT = -2
    };

    static_assert(EnumeratorTraits<Shape, 100>::size() == 4);
    static_assert(EnumeratorTraits<Fruit, 100>::size() == 2);



    static_assert(EnumeratorTraits<Fruit, 100>::nameAt(0) == "APPLE");
    static_assert(EnumeratorTraits<Fruit, 100>::nameAt(1) == "MELON");
    static_assert(EnumeratorTraits<Shape, 100>::nameAt(0) == "POINT");

    std::cout << EnumeratorTraits<Shape, 100>::nameAt(0) << std::endl;


    static_assert(EnumeratorTraits<Shape, 100>::at(0) == Shape::POINT);
    static_assert(EnumeratorTraits<Shape, 100>::at(1) == Shape::SQUARE);
    static_assert(EnumeratorTraits<Shape, 100>::at(3) == Shape::LINE);


    return 0;

}

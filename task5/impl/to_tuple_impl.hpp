//
// Created by gogagum on 20.12.2021.
//

#ifndef TASK5__TO_TUPLE_IMPL_HPP_
#define TASK5__TO_TUPLE_IMPL_HPP_

#include <algorithm>
#include "params_defines.h"

template <class T, std::size_t numFields>
auto toTupleImpl(const T& value) {

    #define GENERATE_LINE(n)  \
    if constexpr(numFields == (n)) \
    { auto &[PARAMS_##n] = value; return std::tie(PARAMS_##n); } else

    if constexpr(numFields == 0) { return std::tuple<>(); } else
    GENERATE_LINE(1)
    GENERATE_LINE(2)
    GENERATE_LINE(3)
    GENERATE_LINE(4)
    GENERATE_LINE(5)
    GENERATE_LINE(6)
    GENERATE_LINE(7)
    GENERATE_LINE(8)
    GENERATE_LINE(9)
    GENERATE_LINE(10)
    GENERATE_LINE(11)
    GENERATE_LINE(12)
    GENERATE_LINE(13)
    GENERATE_LINE(14)
    GENERATE_LINE(15)
    GENERATE_LINE(16)
    GENERATE_LINE(17)
    GENERATE_LINE(18)
    GENERATE_LINE(19)
    GENERATE_LINE(20)
    GENERATE_LINE(21)
    GENERATE_LINE(22)
    GENERATE_LINE(23)
    GENERATE_LINE(24)
    GENERATE_LINE(25)
    GENERATE_LINE(26)
    GENERATE_LINE(27)
    GENERATE_LINE(28)
    GENERATE_LINE(29)
    GENERATE_LINE(30)
    GENERATE_LINE(31)
    GENERATE_LINE(32)
    GENERATE_LINE(33)
    GENERATE_LINE(34)
    GENERATE_LINE(35)
    GENERATE_LINE(36)
    GENERATE_LINE(37)
    GENERATE_LINE(38)
    GENERATE_LINE(39)
    GENERATE_LINE(40)
    GENERATE_LINE(41)
    GENERATE_LINE(42)
    GENERATE_LINE(43)
    GENERATE_LINE(44)
    GENERATE_LINE(45)
    GENERATE_LINE(46)
    GENERATE_LINE(47)
    GENERATE_LINE(48)
    GENERATE_LINE(49)
    GENERATE_LINE(50)
    GENERATE_LINE(51)
    GENERATE_LINE(52)
    GENERATE_LINE(53)
    GENERATE_LINE(54)
    GENERATE_LINE(55)
    GENERATE_LINE(56)
    GENERATE_LINE(57)
    GENERATE_LINE(58)
    GENERATE_LINE(59)
    GENERATE_LINE(60)
    GENERATE_LINE(61)
    GENERATE_LINE(62)
    GENERATE_LINE(63)
    GENERATE_LINE(64)
    GENERATE_LINE(65)
    GENERATE_LINE(66)
    GENERATE_LINE(67)
    GENERATE_LINE(68)
    GENERATE_LINE(69)
    GENERATE_LINE(70)
    GENERATE_LINE(71)
    GENERATE_LINE(72)
    GENERATE_LINE(73)
    GENERATE_LINE(74)
    GENERATE_LINE(75)
    GENERATE_LINE(76)
    GENERATE_LINE(77)
    GENERATE_LINE(78)
    GENERATE_LINE(79)
    GENERATE_LINE(80)
    GENERATE_LINE(81)
    GENERATE_LINE(82)
    GENERATE_LINE(83)
    GENERATE_LINE(84)
    GENERATE_LINE(85)
    GENERATE_LINE(86)
    GENERATE_LINE(87)
    GENERATE_LINE(88)
    GENERATE_LINE(89)
    GENERATE_LINE(90)
    GENERATE_LINE(91)
    GENERATE_LINE(92)
    GENERATE_LINE(93)
    GENERATE_LINE(94)
    GENERATE_LINE(95)
    GENERATE_LINE(96)
    GENERATE_LINE(97)
    GENERATE_LINE(98)
    GENERATE_LINE(99)
    GENERATE_LINE(100)
    GENERATE_LINE(101)
    GENERATE_LINE(102)
    GENERATE_LINE(103)
    GENERATE_LINE(104)
    GENERATE_LINE(105)
    GENERATE_LINE(106)
    GENERATE_LINE(107)
    GENERATE_LINE(108)
    GENERATE_LINE(109)
    GENERATE_LINE(110)
    GENERATE_LINE(111)
    GENERATE_LINE(112)
    GENERATE_LINE(113)
    GENERATE_LINE(114)
    GENERATE_LINE(115)
    GENERATE_LINE(116)
    GENERATE_LINE(117)
    GENERATE_LINE(118)
    #undef GENERATE_LINE
    return std::tie();
}


#endif //TASK5__TO_TUPLE_IMPL_HPP_
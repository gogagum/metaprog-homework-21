//
// Created by gogagum on 06.10.2021.
//

#include <optional>

#ifndef TASK01__MAPPINGTOOLS_HPP_
#define TASK01__MAPPINGTOOLS_HPP_

template <class From, auto target>
struct Mapping;

template <class Base, class Target, class... Mappings>
struct ClassMapper {
  static std::optional<Target> map(const Base& object);
};

template <class Base, class Target, class... Mappings>
std::optional<Target>
ClassMapper<Base, Target, Mappings>::map(const Base& object) {

}



#endif //TASK01__MAPPINGTOOLS_HPP_

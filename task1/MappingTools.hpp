//
// Created by gogagum on 06.10.2021.
//

#include <optional>
#include <iostream>

#ifndef TASK01__MAPPINGTOOLS_HPP_
#define TASK01__MAPPINGTOOLS_HPP_

template <class From, auto target>
struct Mapping {
  using From_ = From;
  static constexpr auto getTarget() -> decltype(target) {
      return target;
  };
};

////////////////////////////////////////////////////////////////////////////////
// ClassMapper - class from task.
template <class Base, class Target, class... Mappings>
class ClassMapper {

 private:
  template<class TakenMapping, class... Mappings_>
  struct MapImpl;

 public:
  static std::optional<Target> map(const Base& object) {
      return MapImpl<Mappings...>::mapImpl(object);
  }

 private:

  template <class TakenMapping, class... Mappings_>
  struct MapImpl {
    static std::optional<Target> mapImpl(const Base& object) {
        auto* objPtr = dynamic_cast<const typename TakenMapping::From_*>(&object);
        if (objPtr) {
            return std::make_optional(TakenMapping::getTarget());
        }
        return MapImpl<Mappings_...>::mapImpl(object);
    }
  };

  template <class TakenMapping>
  struct MapImpl<TakenMapping> {
    static std::optional<Target> mapImpl(const Base& object) {
        auto* objPtr = dynamic_cast<const typename TakenMapping::From_*>(&object);
        if (objPtr) {
            return std::make_optional(TakenMapping::getTarget());
        }
        return std::nullopt;
    }
  };
};

#endif //TASK01__MAPPINGTOOLS_HPP_

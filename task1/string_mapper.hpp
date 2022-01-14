//
// Created by gogagum on 06.10.2021.
//

#include <optional>
#include <iostream>
#include <array>
#include <cstring>
#include <string_view>

#ifndef TASK1__STRING_MAPPER_HPP_
#define TASK1__STRING_MAPPER_HPP_

////////////////////////////////////////////////////////////////////////////////
// Mapping
template <class From, auto target>
struct Mapping {
  using From_ = From;
  static constexpr auto getTarget() -> decltype(target) {
      return target;
  };
};

template <class TakenMapping, class Base, class Target>
concept CorrectMapping =
    std::is_same_v<
        TakenMapping,
        Mapping<
            typename TakenMapping::From_,
            TakenMapping::getTarget()
        >
    >
    &&
    std::derived_from<typename TakenMapping::From_, Base>
    &&
    (
        std::is_same_v<decltype(TakenMapping::getTarget()), Target>
        ||
        std::is_same_v<decltype(TakenMapping::getTarget()), const Target>
    )
;


////////////////////////////////////////////////////////////////////////////////
// ClassMapper - class from task.
template <class Base, class Target, class... Mappings>
class ClassMapper {

 private:
  template<CorrectMapping<Base, Target> TakenMapping, class... Mappings_>
  struct MapImpl;

 public:
  static std::optional<Target> map(const Base& object) {
      return MapImpl<Mappings...>::mapImpl(object);
  }

 private:

  template <CorrectMapping<Base, Target> TakenMapping, class... Mappings_>
  struct MapImpl {
    static std::optional<Target> mapImpl(const Base& object) {
        auto* objPtr = dynamic_cast<const typename TakenMapping::From_*>(&object);
        if (objPtr) {
            return std::make_optional(TakenMapping::getTarget());
        }
        return MapImpl<Mappings_...>::mapImpl(object);
    }
  };

  template <CorrectMapping<Base, Target> TakenMapping>
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

////////////////////////////////////////////////////////////////////////////////
// String
template <size_t max_length>
struct String {
    constexpr String(const char* str, size_t len) {
        _len = len;
        for (size_t i = 0; i < len; ++i)
        {
            _chars[i] = str[i];
        }
    }
    constexpr operator std::string_view () const {
        return {_chars, _len};
    }

    size_t _len;
    char _chars[max_length];
};

constexpr String<256> operator ""_cstr(const char* str, size_t len) {
    return {str, len};
}

#endif //TASK1__STRING_MAPPER_HPP_

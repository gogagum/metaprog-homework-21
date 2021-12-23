//
// Created by gogagum on 16.12.2021.
//

#ifndef TASK4__ENUM_TRAITS_HPP_
#define TASK4__ENUM_TRAITS_HPP_

#include <cstddef>
#include <type_traits>
#include <bitset>
#include <array>
#include <climits>
#include <vector>
#include <cstring>
#include <limits>
#include <algorithm>

template <auto enumElement>
struct StructWithEnumElementParameter {
  //private:
  /**
   *  Get full template parameter name.
   */
  static constexpr std::string_view getNameImpl() noexcept;
 public:
  /**
   * Find out if there are four dots.
   * @return true if there are four dots in template parameter, else false.
   */
  //static constexpr bool hasFourDots() noexcept;

  static constexpr bool hasParentheses() noexcept;

  /**
   * Get enumerator name.
   * @return enumerator name, or empty string_view if parameter is not enumerator.
   */
  static constexpr std::string_view getName() noexcept;

};

////////////////////////////////////////////////////////////////////////////////
// struct EnumeratorTraits<Enum, size_t>                                      //
template <class Enum, size_t MAXN=512> requires std::is_enum_v<Enum>
struct EnumeratorTraits {
  static constexpr long long start() noexcept {
      using IntType = std::underlying_type_t<Enum>;
      return std::max((long long)std::numeric_limits<IntType>::min(),
                      -(long long)MAXN);
  }

  static constexpr long long end() noexcept {
      using IntType = std::underlying_type_t<Enum>;
      if constexpr (std::is_same_v<IntType, long long>
                 || std::is_same_v<IntType, unsigned long long>) {
          return MAXN + 1;
      } else {
          return std::min((long long)(std::numeric_limits<IntType>::max()),
                          (long long)MAXN) + 1;
      }
  }

  static constexpr std::size_t maxLength = end() - start();

  static constexpr size_t size() noexcept;
  static constexpr Enum at(size_t i) noexcept;
  static constexpr std::string_view nameAt(size_t i) noexcept;

  template <size_t maxLength>
  struct Info {
    std::array<std::string_view, maxLength> namesArray;
    std::array<long long, maxLength> indexesArray;
    size_t actualSize;
  };

  template <long long start, long long finish>
  struct InfoCheckerHelper {
    static constexpr Info<finish - start> checkInfo() noexcept;
  };

  static constexpr Info info = InfoCheckerHelper<start(), end()>::checkInfo();
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class Enum, size_t MAXN> requires std::is_enum_v<Enum>
template <long long start, long long finish>
constexpr auto
EnumeratorTraits<Enum, MAXN>::InfoCheckerHelper<start, finish>::checkInfo() noexcept -> Info<finish - start> {
    if constexpr (finish - start == 0) {
        Info<0> info{};
        return info;
    } else if constexpr(finish - start == 1) {
        if constexpr(!StructWithEnumElementParameter<(Enum)start>::hasParentheses()) {
            constexpr auto indexes = std::array<long long, 1>{start};
            constexpr auto names = std::array<std::string_view, 1>{ StructWithEnumElementParameter<(Enum)start>::getName() };
            return Info<1>{ names, indexes, 1 };
        } else {
            constexpr auto indexes = std::array<long long, 1>{};
            constexpr auto names = std::array<std::string_view, 1>{  };
            return Info<1>{ names, indexes, 0 };
        }
    } else {
        constexpr long long middle = (start + finish) / 2;
        constexpr auto firstHalfIndexesInfo = InfoCheckerHelper<start, middle>::checkInfo();
        constexpr auto secondHalfIndexesInfo = InfoCheckerHelper<middle, finish>::checkInfo();

        auto indexes = std::array<long long, finish - start>();
        auto names = std::array<std::string_view, finish - start>();
        for (size_t i = 0; i < firstHalfIndexesInfo.actualSize; i++) {
            indexes[i] = firstHalfIndexesInfo.indexesArray[i];
            names[i] = firstHalfIndexesInfo.namesArray[i];
        }
        constexpr std::size_t skipped = firstHalfIndexesInfo.actualSize;
        for (size_t i = 0; i < secondHalfIndexesInfo.actualSize; i++) {
            indexes[i + skipped] = secondHalfIndexesInfo.indexesArray[i];
            names[i + skipped] = secondHalfIndexesInfo.namesArray[i];
        }
        constexpr size_t sumLength =
            firstHalfIndexesInfo.actualSize + secondHalfIndexesInfo.actualSize;
        return Info<finish - start>{ names, indexes, sumLength };
    }
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<auto enumElement>
constexpr std::string_view
StructWithEnumElementParameter<enumElement>::getNameImpl() noexcept {
    constexpr std::string_view fullFuncName = __PRETTY_FUNCTION__ ;

#if defined(__clang__)
    constexpr auto start = fullFuncName.find('<');
    constexpr auto finish = fullFuncName.find('>');
#elif defined(__GNUC__) || defined(__GNUG__)
    constexpr auto start = fullFuncName.find("enumElement = ") + 13;
    constexpr auto finish = fullFuncName.find("; std::string_view");
    if constexpr (*(fullFuncName.begin() + start + 1) == '(') {
        return std::string_view{};
    }
#endif
    return std::string_view{ fullFuncName.begin() + start + 1, fullFuncName.begin() + finish };
}

//----------------------------------------------------------------------------//
template<auto enumElement>
constexpr bool StructWithEnumElementParameter<enumElement>::hasParentheses() noexcept {
    constexpr auto parameterName = getNameImpl();
    constexpr auto opening = parameterName.find('(');
    constexpr auto closing = parameterName.find(')');

    return opening != std::string_view::npos
        || closing != std::string_view::npos;
}

//----------------------------------------------------------------------------//
template<auto enumElement>
constexpr std::string_view StructWithEnumElementParameter<enumElement>::getName() noexcept {
    constexpr auto parameterName = getNameImpl();
    constexpr auto dotsIndex = parameterName.find_last_of(':');
    if constexpr (dotsIndex == std::string_view::npos) {
        return parameterName;
    } else {
        return { parameterName.begin() + dotsIndex + 1, parameterName.end() };
    }
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr size_t EnumeratorTraits<Enum, MAXN>::size() noexcept {
    return info.actualSize;
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr Enum EnumeratorTraits<Enum, MAXN>::at(size_t i) noexcept {
    return (Enum)info.indexesArray[i];
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr std::string_view
EnumeratorTraits<Enum, MAXN>::nameAt(size_t i) noexcept {
    return info.namesArray[i];
}

#endif //TASK4__ENUM_TRAITS_HPP_
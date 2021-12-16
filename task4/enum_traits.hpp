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
  static constexpr bool hasFourDots() noexcept;

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
  static constexpr size_t size() noexcept;
  static constexpr Enum at(size_t i) noexcept;
  static constexpr std::string_view nameAt(size_t i) noexcept;

  static constexpr std::array<long long, (long long) MAXN * 2 + 1> getIndexes() noexcept;

  template <size_t maxLength>
  struct IndexesInfo {
    std::array<long long, maxLength> indexesArray;
    size_t actualSize;
  };

  template <long long start, long long finish>
  struct IndexesChecker {
    static const size_t maxLength = finish - start;
    static constexpr IndexesInfo<maxLength> checkIndexes() noexcept;
  };

  static constexpr std::array<std::string_view, MAXN * 2 + 1> getNames() noexcept;

  template <size_t maxLength>
  struct NamesInfo {
    std::array<std::string_view, maxLength> namesArray;
    size_t actualSize;
  };

  template <long long start, long long finish>
  struct NamesChecker {
    static const size_t maxLength = finish - start;
    static constexpr NamesInfo<maxLength> checkNames() noexcept;
  };

  template <long long start, long long finish>
  struct LengthChecker {
    static constexpr size_t checkLength() noexcept;
  };
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
template<long long start, long long finish>
constexpr auto
EnumeratorTraits<Enum, MAXN>::IndexesChecker<start, finish>::checkIndexes() noexcept -> IndexesInfo<maxLength> {
    if constexpr(start == finish) {
        return IndexesInfo{std::array<long long, finish - start>(), 0};
    } else if constexpr(finish - start == 1) {
        if constexpr(StructWithEnumElementParameter<(Enum)start>::hasFourDots()) {
            constexpr auto arr = std::array<long long, 1>{start};
            return IndexesInfo<1>{arr, 1};
        } else {
            constexpr auto arr = std::array<long long, 1>();
            return IndexesInfo<1>{arr, 0};
        }
    } else {
        constexpr long long middle = (start + finish) / 2;
        constexpr auto firstHalfIndexesInfo = IndexesChecker<start, middle>::checkIndexes();
        constexpr auto secondHalfIndexesInfo = IndexesChecker<middle, finish>::checkIndexes();

        auto arr = std::array<long long, finish - start>();
        for (size_t i = 0; i < firstHalfIndexesInfo.actualSize; i++) {
            arr[i] = firstHalfIndexesInfo.indexesArray[i];
        }
        for (size_t i = 0; i < secondHalfIndexesInfo.actualSize; i++) {
            arr[firstHalfIndexesInfo.actualSize + i] = secondHalfIndexesInfo.indexesArray[i];
        }
        constexpr size_t sumLength =
            firstHalfIndexesInfo.actualSize + secondHalfIndexesInfo.actualSize;
        return IndexesInfo<finish - start>{arr, sumLength};
    }
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
template<long long int start, long long int finish>
constexpr auto
EnumeratorTraits<Enum, MAXN>::NamesChecker<start, finish>::checkNames() noexcept -> NamesInfo<maxLength> {
    if constexpr(start == finish) {
        return NamesInfo{std::array<std::string_view, finish - start>(), 0};
    } else if constexpr(finish - start == 1) {
        if constexpr(StructWithEnumElementParameter<(Enum)start>::hasFourDots()) {
            constexpr auto arr =
                std::array<std::string_view, 1>{
                    StructWithEnumElementParameter<(Enum)start>::getName()
                };
            return NamesInfo<1>{arr, 1};
        } else {
            constexpr auto arr = std::array<std::string_view, 1>();
            return NamesInfo<1>{arr, 0};
        }
    } else {
        constexpr long long middle = (start + finish) / 2;
        constexpr auto firstHalfNamesInfo = NamesChecker<start, middle>::checkNames();
        constexpr auto secondHalfNamesInfo = NamesChecker<middle, finish>::checkNames();

        auto arr = std::array<std::string_view, finish - start>();
        for (size_t i = 0; i < firstHalfNamesInfo.actualSize; i++) {
            arr[i] = firstHalfNamesInfo.namesArray[i];
        }
        for (size_t i = 0; i < secondHalfNamesInfo.actualSize; i++) {
            arr[firstHalfNamesInfo.actualSize + i] = secondHalfNamesInfo.namesArray[i];
        }
        constexpr size_t sumLength =
            firstHalfNamesInfo.actualSize + secondHalfNamesInfo.actualSize;
        return NamesInfo<finish - start>{arr, sumLength};
    }
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
template<long long int start, long long int finish>
constexpr size_t
EnumeratorTraits<Enum, MAXN>::LengthChecker<start, finish>::checkLength() noexcept {
    if constexpr(start == finish) {
        return 0;
    } else if constexpr(finish - start == 1) {
        if constexpr(StructWithEnumElementParameter<(Enum)start>::hasFourDots()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        constexpr long long middle = (start + finish) / 2;
        constexpr size_t firstHalfLength = LengthChecker<start, middle>::checkLength();
        constexpr size_t secondHalfLength = LengthChecker<middle, finish>::checkLength();

        return firstHalfLength + secondHalfLength;
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
constexpr bool StructWithEnumElementParameter<enumElement>::hasFourDots() noexcept {
    constexpr auto parameterName = getNameImpl();
    constexpr auto dotsIndex = parameterName.find("::");
    return dotsIndex != std::string_view::npos;
}

//----------------------------------------------------------------------------//
template<auto enumElement>
constexpr std::string_view StructWithEnumElementParameter<enumElement>::getName() noexcept {
    constexpr auto parameterName = getNameImpl();
    constexpr auto dotsIndex = parameterName.find_last_of("::");
    return { parameterName.begin() + dotsIndex + 1, parameterName.end() };
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr size_t EnumeratorTraits<Enum, MAXN>::size() noexcept {
    return LengthChecker<-(long long)MAXN, (long long)MAXN + 1>::checkLength();
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr Enum EnumeratorTraits<Enum, MAXN>::at(size_t i) noexcept {
    constexpr auto arr = getIndexes();
    return (Enum)arr[i];
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr std::string_view
EnumeratorTraits<Enum, MAXN>::nameAt(size_t i) noexcept {
    constexpr auto names = getNames();
    return names[i];
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr std::array<long long, (long long) MAXN * 2 + 1>
EnumeratorTraits<Enum, MAXN>::getIndexes() noexcept {
    return IndexesChecker<-(long long)MAXN, (long long)MAXN + 1>::checkIndexes().indexesArray;
}

//----------------------------------------------------------------------------//
template<class Enum, size_t MAXN> requires std::is_enum_v<Enum>
constexpr std::array<std::string_view, MAXN * 2 + 1>
EnumeratorTraits<Enum, MAXN>::getNames() noexcept {
    return NamesChecker<-(long long)MAXN, (long long)MAXN + 1>::checkNames().namesArray;
}

#endif //TASK4__ENUM_TRAITS_HPP_

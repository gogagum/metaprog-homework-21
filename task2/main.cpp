#include <iostream>

#include "type_lists.hpp"

template <typename T>
struct IsIntegral {
  const static bool Value = std::integral<T>;
};

int main() {

    // Проверки того, что TypeLists::TypeListStruct соответствует концепту
    static_assert(TypeLists::TypeSequence<TypeLists::TypeListStruct<float, TypeLists::Nil>>);

    ////////////////////////////////////////////////////////////////////////////
    // Convertions                                                            //
    ////////////////////////////////////////////////////////////////////////////
    using TestTypeList =
        typename TypeLists::TypeListStruct<
                     float,
                     TypeLists::TypeListStruct<
                         int,
                         TypeLists::TypeListStruct<
                             bool,
                             TypeLists::Nil
                         >
                     >
                 >;
    using TestTypeTuple =
        TypeLists::TTuple<float, int, bool>;

    using TTupleConvertedFromList =
        TypeLists::ConvertToTTuple<TestTypeList>;

    static_assert(std::is_same_v<TTupleConvertedFromList, TestTypeTuple>);

    // Пока что для тестирования будем конвертировать TTuple туда и обратно.
    using TypeListConvertedFromTuple =
        TypeLists::ConvertToTypeList<TestTypeTuple>;

    using TypeTupleConvertedFromConverted =
        TypeLists::ConvertToTTuple<TypeListConvertedFromTuple>;

    static_assert(std::is_same_v<TypeTupleConvertedFromConverted, TestTypeTuple>);

    ////////////////////////////////////////////////////////////////////////////
    // Cons                                                                   //
    ////////////////////////////////////////////////////////////////////////////
    static_assert(
        std::is_same_v<
            TypeLists::TypeListStruct<int, TypeLists::TypeListStruct<float, TypeLists::Nil>>,
            TypeLists::Cons<int, TypeLists::TypeListStruct<float, TypeLists::Nil>>
        >
    );

    ////////////////////////////////////////////////////////////////////////////
    // Repeat                                                                 //
    ////////////////////////////////////////////////////////////////////////////
    static_assert(
        TypeLists::TypeSequence<TypeLists::Repeat<int>>
    );

    using RepeatTestTypeList = TypeLists::Repeat<float>;
    static_assert(std::is_same_v<RepeatTestTypeList::Head, float>);

    using ThreeFloats = TypeLists::Take<3, RepeatTestTypeList>;
    using FiveFloats = TypeLists::Take<5, RepeatTestTypeList>;

    using ThreeFloatsTTuple = TypeLists::ConvertToTTuple<ThreeFloats>;
    using FiveFloatsTTuple = TypeLists::ConvertToTTuple<FiveFloats>;

    static_assert(std::is_same_v<ThreeFloatsTTuple,
                                 TypeLists::TTuple<float, float, float>>);

    static_assert(std::is_same_v<FiveFloatsTTuple,
                                 TypeLists::TTuple<float, float, float, float, float>>);

    static_assert(std::is_same_v<RepeatTestTypeList,
                                 typename RepeatTestTypeList::Tail>);

    ////////////////////////////////////////////////////////////////////////////
    // Take                                                                   //
    ////////////////////////////////////////////////////////////////////////////
    using Seq = TypeLists::Repeat<int>;

    using Taken = TypeLists::Take<5, Seq>;

    using TakenToTTuple = TypeLists::ConvertToTTuple<Taken>;
    static_assert(std::is_same_v<TakenToTTuple, TypeLists::TTuple<int, int, int, int, int>>);

    ////////////////////////////////////////////////////////////////////////////
    // Drop                                                                   //
    ////////////////////////////////////////////////////////////////////////////
    using DropTestTuple = TypeLists::TTuple<int, float, double, long long>;
    using DropTestList = TypeLists::ConvertToTypeList<DropTestTuple>;
    using DropResult = TypeLists::Drop<2, DropTestList>;
    using DropResultTTuple = TypeLists::ConvertToTTuple<DropResult>;

    static_assert(std::is_same_v<DropResultTTuple, TypeLists::TTuple<double, long long>>);
    static_assert(std::is_same_v<DropResult::Head, double>);
    static_assert(std::is_same_v<DropResult::Tail::Head, long long>);

    ////////////////////////////////////////////////////////////////////////////
    // Replicate                                                              //
    ////////////////////////////////////////////////////////////////////////////

    using Replicated = TypeLists::Replicate<2, size_t>;

    static_assert(std::is_same_v<Replicated::Head, size_t>);
    static_assert(std::is_same_v<Replicated::Tail::Head, size_t>);

    using ReplicatedToTTuple = TypeLists::ConvertToTTuple<Replicated>;

    static_assert(std::is_same_v<ReplicatedToTTuple,
                                 TypeLists::TTuple<size_t, size_t>>);

    ////////////////////////////////////////////////////////////////////////////
    // Filter                                                                 //
    ////////////////////////////////////////////////////////////////////////////

    using ToFilterTuple1 = TypeLists::TTuple<float, double, double>;
    using ToFilterList1 = TypeLists::ConvertToTypeList<ToFilterTuple1>;

    using FilteredList1 = TypeLists::Filter<IsIntegral, ToFilterList1>;

    using FilteredListToTuple1 = TypeLists::ConvertToTTuple<FilteredList1>;

    static_assert(std::is_same_v<FilteredListToTuple1,
                                 TypeLists::TTuple<>>);

    using ToFilterTuple2 = TypeLists::TTuple<char, float, int>;
    using ToFilterList2 = TypeLists::ConvertToTypeList<ToFilterTuple2>;

    using FilteredList2 = TypeLists::Filter<IsIntegral, ToFilterList2>;

    using FilteredListToTuple2 = TypeLists::ConvertToTTuple<FilteredList2>;

    static_assert(std::is_same_v<FilteredListToTuple2,
                                 TypeLists::TTuple<char, int>>);



    return 0;
}

#include <iostream>
#include <memory>

#include "type_lists.hpp"

template <typename T>
struct IsIntegral {
  const static bool Value = std::integral<T>;
};

template <typename T1, typename T2>
struct IsSame {
  const static bool Value = std::is_same_v<T1, T2>;
};

int main() {

    // Проверки того, что TypeLists::TypeListStruct соответствует концепту
    static_assert(TypeLists::TypeSequence<TypeLists::TypeListStruct<float, TypeLists::Nil>>);

    using SimpleTestTTuple = TypeLists::TTuple<int, float, double>;
    using SimpleTestTypeList = TypeLists::ConvertToTypeList<SimpleTestTTuple>;

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

    static_assert(std::is_same_v<Taken::Head, int>);
    static_assert(std::is_same_v<Taken::Tail::Head, int>);

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

    ////////////////////////////////////////////////////////////////////////////
    // Map                                                                    //
    ////////////////////////////////////////////////////////////////////////////

    using MapResult = TypeLists::Map<std::shared_ptr, SimpleTestTypeList>;

    using MapResultTTuple = TypeLists::ConvertToTTuple<MapResult>;

    static_assert(std::is_same_v<MapResultTTuple, TypeLists::TTuple<std::shared_ptr<int>,
                                                                    std::shared_ptr<float>,
                                                                    std::shared_ptr<double>>>);

    ////////////////////////////////////////////////////////////////////////////
    // Iterate                                                                //
    ////////////////////////////////////////////////////////////////////////////

    using SuperPtrType = TypeLists::Iterate<std::shared_ptr, int>;

    static_assert(std::is_same_v<SuperPtrType::Head, int>);
    static_assert(std::is_same_v<SuperPtrType::Tail::Head, std::shared_ptr<int>>);
    static_assert(std::is_same_v<SuperPtrType::Tail::Tail::Head, std::shared_ptr<std::shared_ptr<int>>>);

    ////////////////////////////////////////////////////////////////////////////
    // Cycle                                                                  //
    ////////////////////////////////////////////////////////////////////////////


    using TestCycleResult = TypeLists::Cycle<SimpleTestTypeList>;

    using TestCycleResultCutted = TypeLists::Take<7, TestCycleResult>;
    using TestCycleResultTTuple = TypeLists::ConvertToTTuple<TestCycleResultCutted>;

    static_assert(std::is_same_v<TestCycleResultTTuple,
                                 TypeLists::TTuple<int,
                                                   float,
                                                   double,
                                                   int,
                                                   float,
                                                   double,
                                                   int>>);

    ////////////////////////////////////////////////////////////////////////////
    // Inits                                                                  //
    ////////////////////////////////////////////////////////////////////////////

    using InitsResult = TypeLists::Inits<SimpleTestTypeList>;

    using FirstInit = InitsResult::Head;
    using FirstInitTTuple = TypeLists::ConvertToTTuple<FirstInit>;

    static_assert(std::is_same_v<FirstInitTTuple, TypeLists::TTuple<int>>);

    using SecondInit = InitsResult::Tail::Head;
    using SecondInitTTuple = TypeLists::ConvertToTTuple<SecondInit>;

    static_assert(std::is_same_v<SecondInitTTuple, TypeLists::TTuple<int, float>>);

    using ThirdInit = InitsResult::Tail::Tail::Head;
    using ThirdInitTTuple = TypeLists::ConvertToTTuple<ThirdInit>;

    static_assert(std::is_same_v<ThirdInitTTuple, TypeLists::TTuple<int, float, double>>);

    ////////////////////////////////////////////////////////////////////////////
    // Tails                                                                  //
    ////////////////////////////////////////////////////////////////////////////

    using TailsResult = TypeLists::Tails<SimpleTestTypeList>;

    using FirstTail = TailsResult::Head;
    using FirstTailTTuple = TypeLists::ConvertToTTuple<FirstTail>;

    static_assert(std::is_same_v<FirstTailTTuple, TypeLists::TTuple<float, double>>);

    using SecondTail = TailsResult::Tail::Head;
    using SecondTailTTuple = TypeLists::ConvertToTTuple<SecondTail>;

    static_assert(std::is_same_v<SecondTailTTuple, TypeLists::TTuple<double>>);

    ////////////////////////////////////////////////////////////////////////////
    // Scanl                                                                  //
    ////////////////////////////////////////////////////////////////////////////

    using ScanlTestTTuple = TypeLists::TTuple<float, double>;
    using ScanlTestTypeList = TypeLists::ConvertToTypeList<ScanlTestTTuple>;

    using ScanlResult = TypeLists::Scanl<std::pair, int, ScanlTestTypeList>;

    using ScanlResultTTuple = TypeLists::ConvertToTTuple<ScanlResult>;

    static_assert(std::is_same_v<ScanlResultTTuple,
                                 TypeLists::TTuple<std::pair<int, float>,
                                                   std::pair<std::pair<int, float>, double>>>);


    ////////////////////////////////////////////////////////////////////////////
    // Foldl                                                                  //
    ////////////////////////////////////////////////////////////////////////////

    using FoldlResult = TypeLists::Foldl<std::pair, int, SimpleTestTypeList>;

    static_assert(std::is_same_v<
                      FoldlResult,
                      std::pair<
                          std::pair<
                              std::pair<int, int>,
                              float
                          >,
                      double
                  >>);

    ////////////////////////////////////////////////////////////////////////////
    // Zip2                                                                   //
    ////////////////////////////////////////////////////////////////////////////

    using FirstTTupleToZip = TypeLists::TTuple<int, float, double>;
    using SecondTTupleToZip = TypeLists::TTuple<double, float>;

    using FirstTypeListToZip = TypeLists::ConvertToTypeList<FirstTTupleToZip>;
    using SecondTypeListToZip = TypeLists::ConvertToTypeList<SecondTTupleToZip>;

    using Zip2Result = TypeLists::Zip2<FirstTypeListToZip, SecondTypeListToZip>;

    using Zip2ResultWithTTupleElements = TypeLists::Map<TypeLists::ConvertToTTuple, Zip2Result>;

    using Zip2ResultAsTupleOfTuples = TypeLists::ConvertToTTuple<Zip2ResultWithTTupleElements>;

    static_assert(std::is_same_v<Zip2ResultAsTupleOfTuples,
                                 TypeLists::TTuple<
                                     TypeLists::TTuple<int, double>,
                                     TypeLists::TTuple<float, float>
                                 >>);

    ////////////////////////////////////////////////////////////////////////////
    // Zip                                                                    //
    ////////////////////////////////////////////////////////////////////////////

    using ThirdTTupleToZip = TypeLists::TTuple<size_t, ssize_t>;

    using ThirdTypeListToZip = TypeLists::ConvertToTypeList<ThirdTTupleToZip>;

    using ZipResult = TypeLists::Zip<FirstTypeListToZip, SecondTypeListToZip, ThirdTypeListToZip>;

    using ZipResultWithTTupleElements = TypeLists::Map<TypeLists::ConvertToTTuple, ZipResult>;

    using ZipResultAsTupleOfTuples = TypeLists::ConvertToTTuple<ZipResultWithTTupleElements>;

    static_assert(std::is_same_v<ZipResultAsTupleOfTuples,
                                 TypeLists::TTuple<
                                     TypeLists::TTuple<int, double, size_t>,
                                     TypeLists::TTuple<float, float, ssize_t>
                                 >>);

    ////////////////////////////////////////////////////////////////////////////
    // GroupBy                                                                //
    ////////////////////////////////////////////////////////////////////////////

    using GroupByTestInputTTuple = TypeLists::TTuple<float, int, int>;
    using GroupByTestInputTypeList =
        TypeLists::ConvertToTypeList<GroupByTestInputTTuple>;

    using GroupByResult = TypeLists::GroupBy<IsSame, GroupByTestInputTypeList>;

    using GroupByResultAsTTuples = TypeLists::Map<TypeLists::ConvertToTTuple, GroupByResult>;

    using GroupByResultAsTupleOfTuples = TypeLists::ConvertToTTuple<GroupByResultAsTTuples>;

    static_assert(std::is_same_v<GroupByResultAsTupleOfTuples,
                                 TypeLists::TTuple<
                                     TypeLists::TTuple<float>,
                                     TypeLists::TTuple<int, int>
                                 >>);

    return 0;
}

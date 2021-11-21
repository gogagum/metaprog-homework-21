#include <iostream>

#include <memory>
#include <vector>

#include "type_lists.hpp"

template <typename T>
struct IsIntegral {
    const static bool Value = std::integral<T>;
};

template <typename T1, typename T2>
struct IsSame {
    const static bool Value = std::is_same_v<T1, T2>;
};

template <class T>
using NotIsIntegral = TypeLists::ApplyLogicNotToValue<IsIntegral<T>>;

template <typename T1, typename T2>
struct MakePair {
  using Type = std::pair<T1, T2>;
};

template<auto V>
struct ValueTag{ static constexpr auto Value = V; };

template<typename T>
struct AddOne{ static constexpr auto Value = T::Value + 1; };

template<class F, class S>
struct Sum {
  static constexpr auto Value = F::Value + S::Value;
};

template<typename F, typename S>
struct LastTwoAdder {
 private:
  using Sum_ = Sum<F, S>;
 public:
  using Head = Sum_;
  using Tail = LastTwoAdder<S, Sum_>;
};

using Fibs = TypeLists::Cons<ValueTag<0>, LastTwoAdder<ValueTag<0>, ValueTag<1>>>;

template<TypeLists::TypeSequence TailNumbers>
struct PrimesImpl{
  template <typename T>
  struct Erase {
      const static bool Value = T::Value % TailNumbers::Head::Value != 0;
  };

  using Head = typename TailNumbers::Head;
  using Tail = PrimesImpl<TypeLists::Filter<Erase, typename TailNumbers::Tail>>;
};


int main() {

    // Проверки того, что TypeLists::TypeListStruct соответствует концепту
    static_assert(TypeLists::TypeSequence<TypeLists::Cons<float, TypeLists::Nil>>);

    // int, float, double
    using SimpleTestTTuple = TypeLists::TTuple<int, float, double>;

    // int, float, double
    using SimpleTestTypeList =
    TypeLists::Cons<
            int,
            TypeLists::Cons<
                    float,
                    TypeLists::Cons<
                            double,
                            TypeLists::Nil
                    >
            >
    >;

    // int, float, double, int, float, double ...
    using SimpleTestCycleTypeList = TypeLists::Cycle<SimpleTestTypeList>;

    ////////////////////////////////////////////////////////////////////////////
    // Convertions                                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Convert TTuple<int, float, double> to TypeList
    using TTupleConvertedFromSimpleTestTypeList =
    TypeLists::ConvertToTTuple<SimpleTestTypeList>;

    static_assert(std::is_same_v<TTupleConvertedFromSimpleTestTypeList,
            SimpleTestTTuple>);

    // Пока что для тестирования будем конвертировать TTuple туда и обратно.
    using TypeListConvertedFromTuple =
    TypeLists::ConvertToTypeList<SimpleTestTTuple>;

    using TypeTupleConvertedFromConverted =
    TypeLists::ConvertToTTuple<TypeListConvertedFromTuple>;

    static_assert(std::is_same_v<TypeTupleConvertedFromConverted,
            SimpleTestTTuple>);

    ////////////////////////////////////////////////////////////////////////////
    // Concat                                                                 //
    ////////////////////////////////////////////////////////////////////////////
    using ConcatResult = TypeLists::Concat<SimpleTestTypeList, SimpleTestTypeList>;
    using ConcatResultTTuple = TypeLists::ConvertToTTuple<ConcatResult>;

    static_assert(std::is_same_v<typename ConcatResult::Head, int>);
    static_assert(std::is_same_v<typename ConcatResult::Tail::Head, float>);

    static_assert(std::is_same_v<int, TypeLists::GetFromTypeList<3, ConcatResult>>);

    static_assert(TypeLists::LengthGreaterOrEqual<ConcatResult , 6>);
    static_assert(!TypeLists::LengthGreaterOrEqual<ConcatResult , 7>);

    static_assert(std::is_same_v<ConcatResultTTuple,
            TypeLists::TTuple<int, float, double, int, float, double>>);

    ////////////////////////////////////////////////////////////////////////////
    // Cons                                                                   //
    ////////////////////////////////////////////////////////////////////////////
    using ConsResult = TypeLists::Cons<bool, SimpleTestTypeList>;
    using ConsResultTTuple = TypeLists::ConvertToTTuple<ConsResult>;

    static_assert(
            std::is_same_v<
                    ConsResultTTuple,
                    TypeLists::TTuple<bool, int, float, double>
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

    static_assert(TypeLists::LengthGreaterOrEqual<RepeatTestTypeList, 104>);
    static_assert(std::is_same_v<TypeLists::GetFromTypeList<99, RepeatTestTypeList>, float>);

    ////////////////////////////////////////////////////////////////////////////
    // Take                                                                   //
    ////////////////////////////////////////////////////////////////////////////
    using Seq = TypeLists::Repeat<int>;

    using Taken = TypeLists::Take<5, Seq>;

    static_assert(TypeLists::LengthGreaterOrEqual<Taken, 5>);
    static_assert(!TypeLists::LengthGreaterOrEqual<Taken, 6>);

    using TakenToTTuple = TypeLists::ConvertToTTuple<Taken>;
    static_assert(std::is_same_v<TakenToTTuple, TypeLists::TTuple<int, int, int, int, int>>);

    static_assert(std::is_same_v<Taken::Head, int>);
    static_assert(std::is_same_v<Taken::Tail::Head, int>);

    ////////////////////////////////////////////////////////////////////////////
    // GetFromTTuple                                                          //
    ////////////////////////////////////////////////////////////////////////////

    using SingleTTuple = TypeLists::TTuple<int>;

    static_assert(std::is_same_v<int, TypeLists::GetFromTTuple<0, SingleTTuple>>);

    static_assert(std::is_same_v<int, TypeLists::GetFromTTuple<0, SimpleTestTTuple>>);
    static_assert(std::is_same_v<float, TypeLists::GetFromTTuple<1, SimpleTestTTuple>>);
    static_assert(std::is_same_v<double, TypeLists::GetFromTTuple<2, SimpleTestTTuple>>);

    static_assert(std::is_same_v<float, TypeLists::GetFromTypeList<130, SimpleTestCycleTypeList>>);

    ////////////////////////////////////////////////////////////////////////////
    // GetFromTTupleWithErrorReturn                                           //
    ////////////////////////////////////////////////////////////////////////////

    static_assert(std::is_same_v<int, TypeLists::GetFromTTupleWithErrorReturn<0, SingleTTuple>>);
    static_assert(std::is_same_v<TypeLists::ErrorReturn,
            TypeLists::GetFromTTupleWithErrorReturn<1, SingleTTuple>>);

    static_assert(std::is_same_v<int, TypeLists::GetFromTTupleWithErrorReturn<0, SimpleTestTTuple>>);
    static_assert(std::is_same_v<float, TypeLists::GetFromTTupleWithErrorReturn<1, SimpleTestTTuple>>);
    static_assert(std::is_same_v<double, TypeLists::GetFromTTupleWithErrorReturn<2, SimpleTestTTuple>>);

    static_assert(std::is_same_v<TypeLists::ErrorReturn,
            TypeLists::GetFromTTupleWithErrorReturn<42, SimpleTestTTuple>>);

    ////////////////////////////////////////////////////////////////////////////
    // GetFromTypeList                                                        //
    ////////////////////////////////////////////////////////////////////////////

    static_assert(std::is_same_v<TypeLists::GetFromTypeList<0, SimpleTestTypeList>, int>);
    static_assert(std::is_same_v<TypeLists::GetFromTypeList<1, SimpleTestTypeList>, float>);

    // The following line does not compile.
    //using GetAfterLastElement = TypeLists::GetFromTypeList<4, SimpleTestTypeList>;

    ////////////////////////////////////////////////////////////////////////////
    // concept RequireLengthGreaterOrEqual                                    //
    ////////////////////////////////////////////////////////////////////////////

    static_assert(TypeLists::LengthGreaterOrEqual<SimpleTestCycleTypeList, 3>);

    static_assert(TypeLists::LengthGreaterOrEqual<SimpleTestTypeList, 2>);
    static_assert(TypeLists::LengthGreaterOrEqual<SimpleTestTypeList, 3>);

    using ElementWithIndex3 = TypeLists::GetFromTypeListWithErrorReturn<3, SimpleTestTypeList>;

    static_assert(std::is_same_v<ElementWithIndex3, TypeLists::ErrorReturn>);

    static_assert(!TypeLists::LengthGreaterOrEqual<SimpleTestTypeList, 4>);
    static_assert(TypeLists::LengthGreaterOrEqual<SimpleTestCycleTypeList, 222>);

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

    using DroppedCycle = TypeLists::Drop<27, SimpleTestCycleTypeList>;
    static_assert(std::is_same_v<DroppedCycle::Head,
            SimpleTestCycleTypeList::Head>);
    static_assert(std::is_same_v<DroppedCycle::Tail::Head,
            SimpleTestCycleTypeList::Tail::Head>);

    ////////////////////////////////////////////////////////////////////////////
    // Replicate                                                              //
    ////////////////////////////////////////////////////////////////////////////

    using Replicated = TypeLists::Replicate<2, size_t>;

    static_assert(std::is_same_v<Replicated::Head, size_t>);
    static_assert(std::is_same_v<Replicated::Tail::Head, size_t>);
    static_assert(TypeLists::Empty<Replicated::Tail::Tail>);

    using ReplicatedToTTuple = TypeLists::ConvertToTTuple<Replicated>;

    static_assert(std::is_same_v<ReplicatedToTTuple,
            TypeLists::TTuple<size_t, size_t>>);

    ////////////////////////////////////////////////////////////////////////////
    // ApplyLogicNotToValue                                                   //
    ////////////////////////////////////////////////////////////////////////////

    static_assert(IsIntegral<int>::Value);

    // NotIsIntegral uses ApplyLogicNotToValue
    static_assert(NotIsIntegral<float>::Value);

    ////////////////////////////////////////////////////////////////////////////
    // SkipWhile

    using SimpleTypeListSkippedNotIntegral = TypeLists::SkipWhile<NotIsIntegral, SimpleTestTypeList>;
    using SimpleTypeListSkippedIntegral = TypeLists::SkipWhile<IsIntegral, SimpleTestTypeList>;

    static_assert(std::is_same_v<SimpleTypeListSkippedNotIntegral::Head, int>); // Nothing skipped
    static_assert(std::is_same_v<SimpleTypeListSkippedIntegral::Head, float>);


    ////////////////////////////////////////////////////////////////////////////
    // Filter                                                                 //
    ////////////////////////////////////////////////////////////////////////////

    using ToFilterTuple = TypeLists::TTuple<float, double, double>;
    using ToFilterList = TypeLists::ConvertToTypeList<ToFilterTuple>;

    static_assert(std::is_same_v<ToFilterList::Head, float>);
    static_assert(std::is_same_v<ToFilterList::Tail::Head, double>);
    static_assert(std::is_same_v<ToFilterList::Tail::Tail::Head, double>);
    static_assert(std::is_same_v<ToFilterList::Tail::Tail::Tail, TypeLists::Nil>);

    using FilteredList1 = TypeLists::Filter<IsIntegral, ToFilterList>;

    using FilteredListToTuple1 = TypeLists::ConvertToTTuple<FilteredList1>;

    static_assert(std::is_same_v<FilteredListToTuple1,
                                 TypeLists::TTuple<>>);

    using ToFilterTuple2 = TypeLists::TTuple<char, float, int>;
    using ToFilterList2 = TypeLists::ConvertToTypeList<ToFilterTuple2>;

    using FilteredList2 = TypeLists::Filter<IsIntegral, ToFilterList2>;

    using FilteredListToTuple2 = TypeLists::ConvertToTTuple<FilteredList2>;

    static_assert(std::is_same_v<FilteredListToTuple2,
            TypeLists::TTuple<char, int>>);


    using InfFilterResult = TypeLists::Filter<IsIntegral, SimpleTestCycleTypeList>;

    static_assert(std::is_same_v<InfFilterResult::Head, int>);
    static_assert(std::is_same_v<TypeLists::GetFromTypeList<42, InfFilterResult>, int>);

    ////////////////////////////////////////////////////////////////////////////
    // Map                                                                    //
    ////////////////////////////////////////////////////////////////////////////

    using MapResult = TypeLists::Map<std::shared_ptr, SimpleTestTypeList>;

    using MapResultTTuple = TypeLists::ConvertToTTuple<MapResult>;

    static_assert(std::is_same_v<MapResultTTuple,
            TypeLists::TTuple<std::shared_ptr<int>,
                    std::shared_ptr<float>,
                    std::shared_ptr<double>>>);

    using InfMapResult = TypeLists::Map<std::weak_ptr, SimpleTestCycleTypeList>;

    static_assert(std::is_same_v<TypeLists::GetFromTypeList<42, InfMapResult>, std::weak_ptr<int>>);

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

    static_assert(std::is_same_v<SimpleTestCycleTypeList::Head, int>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Head, float>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Tail::Head, double>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Tail::Tail::Head, int>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Tail::Tail::Tail::Head, float>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Tail::Tail::Tail::Tail::Head, double>);
    static_assert(std::is_same_v<SimpleTestCycleTypeList::Tail::Tail::Tail::Tail::Tail::Tail::Head, int>);

    using TestCycleResultCutted = TypeLists::Take<7, SimpleTestCycleTypeList>;
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

    using ScanlResult = TypeLists::Scanl<MakePair, int, ScanlTestTypeList>;

    static_assert(std::is_same_v<ScanlResult::Head,
                                 std::pair<int, float>>);
    static_assert(std::is_same_v<ScanlResult::Tail::Head,
                                 std::pair<std::pair<int, float>, double>>);

    using ScanlResultTTuple = TypeLists::ConvertToTTuple<ScanlResult>;

    static_assert(std::is_same_v<ScanlResultTTuple,
                  TypeLists::TTuple<std::pair<int, float>,
                                    std::pair<std::pair<int, float>, double>>>);

    using InfScanlResult = TypeLists::Scanl<MakePair, std::size_t, SimpleTestCycleTypeList>;

    static_assert(std::is_same_v<
                      TypeLists::GetFromTypeList<2, InfScanlResult>,
                          std::pair<
                              std::pair<
                                  std::pair<
                                      std::size_t,
                                      int
                                  >,
                                  float
                              >,
                              double
                          >
                      >);

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

    static_assert(std::is_same_v<Zip2Result::Head::Head, int>);
    static_assert(std::is_same_v<Zip2Result::Tail::Head::Head, float>);

    using Zip2ResultWithTTupleElements = TypeLists::Map<TypeLists::ConvertToTTuple, Zip2Result>;

    using Zip2ResultAsTupleOfTuples = TypeLists::ConvertToTTuple<Zip2ResultWithTTupleElements>;

    static_assert(std::is_same_v<Zip2ResultAsTupleOfTuples,
            TypeLists::TTuple<
                    TypeLists::TTuple<int, double>,
                    TypeLists::TTuple<float, float>
            >>);

    ////////////////////////////////////////////////////////////////////////////
    // Zip2 for inf TypeLists

    // int, float, double
    using FirstTypeListTiLoop =
    TypeLists::Cons<int,
            TypeLists::Cons<float,
                    TypeLists::Cons<double,
                            TypeLists::Nil>>>;

    // char, long long
    using SecondTypeListToLoop =
    TypeLists::Cons<char,
            TypeLists::Cons<long long, TypeLists::Nil>>;

    using FirstTestInfTypeList = TypeLists::Cycle<FirstTypeListTiLoop>;
    using SecondTestInfTypeList = TypeLists::Cycle<SecondTypeListToLoop>;

    static_assert(std::is_same_v<int, TypeLists::GetFromTypeList<0, FirstTestInfTypeList>>);
    static_assert(std::is_same_v<float, TypeLists::GetFromTypeList<1, FirstTestInfTypeList>>);

    using InfZip2Res = TypeLists::Zip2<FirstTestInfTypeList, SecondTestInfTypeList>;

    using FirstFromInfZip2Resilt = TypeLists::GetFromTypeList<0, InfZip2Res>;
    using FourtySecondFromInfZip2Resilt = TypeLists::GetFromTypeList<41, InfZip2Res>;

    using FirstFromInfZip2ResiltToTTuple = TypeLists::ConvertToTTuple<FirstFromInfZip2Resilt>;
    using FourtySecondFromInfZip2ResiltToTTuple = TypeLists::ConvertToTTuple<FourtySecondFromInfZip2Resilt>;

    static_assert(std::is_same_v<FirstFromInfZip2ResiltToTTuple, TypeLists::TTuple<int, char>>);
    static_assert(std::is_same_v<FourtySecondFromInfZip2ResiltToTTuple, TypeLists::TTuple<double, long long>>);


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
    // Infinite Zip

    // int, float, double
    using FirstTestTypeListForZip = SimpleTestTypeList;

    // std::string, int, std::size_t
    using SecondTestTypeListForZip = TypeLists::Cons<
            std::string,
            TypeLists::Cons<
                    std::vector<int>,
                    TypeLists::Cons<
                            std::size_t,
                            TypeLists::Nil
                    >
            >
    >;

    // wchar_t, long double
    using ThirdTestTypeListForZip = TypeLists::Cons<
            wchar_t,
            TypeLists::Cons<
                    long double,
                    TypeLists::Nil
            >
    >;

    using InfZipTL1 = TypeLists::Cycle<FirstTestTypeListForZip>;
    using InfZipTL2 = TypeLists::Cycle<SecondTestTypeListForZip>;
    using InfZipTL3 = TypeLists::Cycle<ThirdTestTypeListForZip>;

    using InfZipResult = TypeLists::Zip<InfZipTL1, InfZipTL2, InfZipTL3>;

    using FirstZippedRawFromInfZip = TypeLists::GetFromTypeList<0, InfZipResult>;

    using FirstZippedRawFromInfZipTTuple = TypeLists::ConvertToTTuple<FirstZippedRawFromInfZip>;

    static_assert(std::is_same_v<FirstZippedRawFromInfZipTTuple, TypeLists::TTuple<int, std::string, wchar_t>>);

    using FourtySecondZippedRawFromInfZip = TypeLists::GetFromTypeList<41, InfZipResult>;

    using FourtySecondZippedRawFromInfZipTTuple = TypeLists::ConvertToTTuple<FourtySecondZippedRawFromInfZip>;

    static_assert(std::is_same_v<FourtySecondZippedRawFromInfZipTTuple, TypeLists::TTuple<double, std::size_t, long double>>);

    ////////////////////////////////////////////////////////////////////////////
    // GroupBy                                                                //
    ////////////////////////////////////////////////////////////////////////////

    using OneTypeList = TypeLists::Cons<int, TypeLists::Nil>;

    using OneTypeGroupedBy = TypeLists::GroupBy<IsSame, OneTypeList>;

    using FirstGroup = typename OneTypeGroupedBy::Head;

    using GroupByTestInputTTuple = TypeLists::TTuple<float, int, int>;
    using GroupByTestInputTypeList =
    TypeLists::ConvertToTypeList<GroupByTestInputTTuple>;

    using GroupByResult = TypeLists::GroupBy<IsSame, GroupByTestInputTypeList>;

    static_assert(std::is_same_v<GroupByResult::Head::Head, float>);
    static_assert(std::is_same_v<GroupByResult::Head::Tail, TypeLists::Nil>);

    static_assert(std::is_same_v<GroupByResult::Tail::Head::Head, int>);
    static_assert(std::is_same_v<GroupByResult::Tail::Head::Tail::Head, int>);
    static_assert(std::is_same_v<GroupByResult::Tail::Head::Tail::Tail, TypeLists::Nil>);

    using GroupByResultAsTTuples = TypeLists::Map<TypeLists::ConvertToTTuple, GroupByResult>;

    using GroupByResultAsTupleOfTuples = TypeLists::ConvertToTTuple<GroupByResultAsTTuples>;

    static_assert(std::is_same_v<GroupByResultAsTupleOfTuples,
            TypeLists::TTuple<
                    TypeLists::TTuple<float>,
                    TypeLists::TTuple<int, int>
            >>);

    using InfGroupByResult = TypeLists::GroupBy<IsSame, SimpleTestCycleTypeList>;

    static_assert(std::is_same_v<TypeLists::GetFromTypeList<0, InfGroupByResult>::Head , int>);
    static_assert(std::is_same_v<TypeLists::GetFromTypeList<1, InfGroupByResult>::Head, float>);
    static_assert(std::is_same_v<TypeLists::GetFromTypeList<2, InfGroupByResult>::Head, double>);

    static_assert(std::is_same_v<TypeLists::GetFromTypeList<54, InfGroupByResult>::Head, int>);

    using Nats = TypeLists::Iterate<AddOne, ValueTag<1>>;

    static_assert(TypeLists::GetFromTypeList<41, Nats>::Value == 42);

    static_assert(TypeLists::GetFromTypeList<3, Fibs>::Value == 3);
    static_assert(TypeLists::GetFromTypeList<21, Fibs>::Value == 17711);


    using Primes = PrimesImpl<Nats::Tail>;

    static_assert(TypeLists::GetFromTypeList<0, Primes>::Value == 2);
    static_assert(TypeLists::GetFromTypeList<1, Primes>::Value == 3);
    static_assert(TypeLists::GetFromTypeList<2, Primes>::Value == 5);
    static_assert(TypeLists::GetFromTypeList<6, Primes>::Value == 17);
}

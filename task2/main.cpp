#include <iostream>

#include "type_lists.hpp"

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



    return 0;
}

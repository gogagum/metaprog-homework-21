//
// Created by gogagum on 21.12.2021.
//

#ifndef TASK5__ANNOTATIONS_FARMING_HPP_
#define TASK5__ANNOTATIONS_FARMING_HPP_

#include "type_lisis.hpp"
#include "annotations_checker.hpp"

////////////////////////////////////////////////////////////////////////////////
// AnnotationConcatWith<Annotation1>
template <class Annotation1>
struct AnnotationConcatWith;

////////////////////////////////////////////////////////////////////////////////
// AnnotationConcatWith<Annotate<Annotations...>>
template <class... Annotations>
struct AnnotationConcatWith<Annotate<Annotations...>> {
 private:
  template <class Annotation2>
  struct ConcatWithImpl;

  template <class... OtherAnnotations>
  struct ConcatWithImpl<Annotate<OtherAnnotations...>> {
    using ret = Annotate<Annotations..., OtherAnnotations...>;
  };
 public:
  template<class Annotation2>
  using ret = typename ConcatWithImpl<Annotation2>::ret;
};

////////////////////////////////////////////////////////////////////////////////
// ConcatAnnotations<Annotation1, Annotation2>
template <class Annotation1, class Annotation2>
struct ConcatAnnotations {
  using Ret = typename AnnotationConcatWith<Annotation1>::template ret<Annotation2>;
};

////////////////////////////////////////////////////////////////////////////////
// GetAnnotations<T>
template<class T>
struct GetAnnotations {
 private:
  template<typename CurrentResult, typename AllFieldsTail, class CurrAnnot>
  struct GetAnnotationsImpl;

  template<typename... CurrentResultTypes,  typename Taken, typename... Other, class CurrAnnot>
  requires IsAnnotation<Taken>::Value
  struct GetAnnotationsImpl<TypeLists::TTuple<CurrentResultTypes...>,
                            TypeLists::TTuple<Taken, Other...>,
                            CurrAnnot> {
   private:
    using CurrentResult = TypeLists::TTuple<CurrentResultTypes...>;
    using OtherTTuple = TypeLists::TTuple<Other...>;
    using NewAnnotation = typename ConcatAnnotations<CurrAnnot, Taken>::Ret;
   public:
    using Ret = typename GetAnnotationsImpl<CurrentResult,
                                            OtherTTuple,
                                            NewAnnotation>::Ret;
  };

  template<typename... CurrentResultTypes, typename Taken, typename... Other, class CurrAnnot>
  requires IsNotAnnotation<Taken>::Value
  struct GetAnnotationsImpl<TypeLists::TTuple<CurrentResultTypes...>,
                            TypeLists::TTuple<Taken, Other...>,
                            CurrAnnot> {
   private:
    using NewResult = TypeLists::TTuple<CurrentResultTypes..., CurrAnnot>;
    using OtherTTuple = TypeLists::TTuple<Other...>;
   public:
    using Ret = typename GetAnnotationsImpl<NewResult,
                                            OtherTTuple,
                                            Annotate<>>::Ret;
  };

  template<typename CurrentResult, class CurrAnnot>
  struct GetAnnotationsImpl<CurrentResult, TypeLists::TTuple<>, CurrAnnot> {
    using Ret = CurrentResult;
  };
 public:
  using Ret =
      typename GetAnnotationsImpl<TypeLists::TTuple<>, typename GetAllFields<T>::Ret , Annotate<>>::Ret;
};



#endif //TASK5__ANNOTATIONS_FARMING_HPP_

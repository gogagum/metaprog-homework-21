//
// Created by gogagum on 20.12.2021.
//

#ifndef TASK5__ANNOTATIONS_CHECKER_HPP_
#define TASK5__ANNOTATIONS_CHECKER_HPP_

#include <optional>
template<class...>
class Annotate;

template<class AnnotationCandidate>
struct IsAnnotation {
  constexpr static bool Value = false;
};

template<class... Annotations>
struct IsAnnotation<Annotate<Annotations...>> {
  constexpr static bool Value = true;
};

template<class AnnotationCandidate>
struct IsNotAnnotation {
  constexpr static bool Value = !IsAnnotation<AnnotationCandidate>::Value;
};

template <class Annotation>
struct AnnotationIsEmpty;

template <class... Annotations>
struct AnnotationIsEmpty<Annotate<Annotations...>> {
  constexpr static bool Value = false;
};

template<>
struct AnnotationIsEmpty<Annotate<>> {
  constexpr static bool Value = true;
};

template <class Annotation, class Flag>
struct AnnotationContains {
  constexpr static bool Value = false;
};

template <class Taken, class... Other, class Flag>
struct AnnotationContains<Annotate<Taken, Other...>, Flag> {
  constexpr static bool Value = std::is_same_v<Taken, Flag>
      || AnnotationContains<Annotate<Other...>, Flag>::Value;
};

template <template<class...> class T, template<class...> class U>
struct are_same_template : std::false_type
{};

template <template<class...> class T>
struct are_same_template<T, T> : std::true_type
{};


template <class Annotation, template<class...> class Flag>
struct AnnotationContainsTemplate {
  constexpr static bool Value = false;
};

template <template<class...> class Taken, class... TakenArgs, class... Other, template<class...> class Flag>
struct AnnotationContainsTemplate<Annotate<Taken<TakenArgs...>, Other...>, Flag> {
  constexpr static bool Value = are_same_template<Taken, Flag>::value
      || AnnotationContainsTemplate<Annotate<Other...>, Flag>::Value;
};

template <class Taken, class... Other, template<class...> class Flag>
struct AnnotationContainsTemplate<Annotate<Taken, Other...>, Flag> {
  constexpr static bool Value = AnnotationContainsTemplate<Annotate<Other...>, Flag>::Value;
};

template <class Annotation, template<class...> class AnnotationTemplate>
struct FindAnnotationTemplate {
  static_assert(!AnnotationIsEmpty<Annotation>::Value);
};

template <template<class...> class Taken, class... TakenArgs, class... Other, template<class...> class AnnotationTemplate>
requires are_same_template<Taken, AnnotationTemplate>::value
struct FindAnnotationTemplate<Annotate<Taken<TakenArgs...>, Other...>, AnnotationTemplate> {
  using Ret = Taken<TakenArgs...>;
};

template <template<class...> class Taken, class... TakenArgs, class... Other, template<class...> class AnnotationTemplate>
requires (!are_same_template<Taken, AnnotationTemplate>::value) &&
         (!std::is_same_v<typename FindAnnotationTemplate<Annotate<Other...>, AnnotationTemplate>::Ret,
                          std::nullptr_t>)
struct FindAnnotationTemplate<Annotate<Taken<TakenArgs...>, Other...>, AnnotationTemplate> {
  using Ret = typename FindAnnotationTemplate<Annotate<Other...>, AnnotationTemplate>::Ret;
};

template <class Taken, class... Other, template<class...> class AnnotationTemplate>
requires (!std::is_same_v<typename FindAnnotationTemplate<Annotate<Other...>, AnnotationTemplate>::Ret,
                          std::nullptr_t>)
struct FindAnnotationTemplate<Annotate<Taken, Other...>, AnnotationTemplate> {
  using Ret = typename FindAnnotationTemplate<Annotate<Other...>, AnnotationTemplate>::Ret;
};

template <template<class...> class AnnotationTemplate>
struct FindAnnotationTemplate<Annotate<>, AnnotationTemplate> {
  using Ret = std::nullptr_t;
};


#endif //TASK5__ANNOTATIONS_CHECKER_HPP_

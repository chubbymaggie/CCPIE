#ifndef __PIE_TRAITS_H__
#define __PIE_TRAITS_H__

#include <type_traits>
#include <utility>

#include <boost/mpl/vector.hpp>

#include <boost/tti/has_member_function.hpp>
#include <boost/tti/has_type.hpp>

namespace pie {
namespace traits {

namespace type_categories {

using Simple = std::integral_constant<unsigned, 0>;
using Integer = std::integral_constant<unsigned, 1>;
using Inserter = std::integral_constant<unsigned, 2>;
using BackPusher = std::integral_constant<unsigned, 4>;
using PairInserter = std::integral_constant<unsigned, 8>;

} // namespace type_categories

BOOST_TTI_HAS_TYPE(iterator)
BOOST_TTI_HAS_TYPE(value_type)

BOOST_TTI_HAS_MEMBER_FUNCTION(insert)
BOOST_TTI_HAS_MEMBER_FUNCTION(push_back)

template <typename T, bool = has_type_value_type<T>::value>
struct has_member_function_push_back_safe;

template <typename T>
struct has_member_function_push_back_safe<T, false> {
  static constexpr bool value = false;
};

template <typename T>
struct has_member_function_push_back_safe<T, true> {
  static constexpr bool value = has_member_function_push_back<
      T,
      void,
      boost::mpl::vector<const typename T::value_type &>>::value;
};

template <typename T,
          bool = has_type_value_type<T>::value && has_type_iterator<T>::value>
struct has_member_function_insert_safe;

template <typename T>
struct has_member_function_insert_safe<T, false> {
  static constexpr bool value = false;
};

template <typename T>
struct has_member_function_insert_safe<T, true> {
  static constexpr bool value = has_member_function_insert<
      T,
      std::pair<typename T::iterator, bool>,
      boost::mpl::vector<const typename T::value_type &>>::value;
};

BOOST_TTI_HAS_TYPE(first_type)
BOOST_TTI_HAS_TYPE(second_type)

template <typename T, bool = has_type_value_type<T>::value>
struct has_paired_value_type;

template <typename T>
struct has_paired_value_type<T, false> {
  static constexpr bool value = false;
};

template <typename T>
struct has_paired_value_type<T, true> {
  static constexpr bool value =
      has_type_first_type<typename T::value_type>::value &&
      has_type_second_type<typename T::value_type>::value;
};

template <typename T>
using TypePropertyCodeSum = typename std::integral_constant<
    unsigned,
    (std::is_integral<T>::value ? type_categories::Integer::value
                                : type_categories::Simple::value) +
        (has_member_function_push_back_safe<T>::value
             ? type_categories::BackPusher::value
             : type_categories::Simple::value) +
        (has_member_function_insert_safe<T>::value
             ? (has_paired_value_type<T>::value
                    ? type_categories::PairInserter::value
                    : type_categories::Inserter::value)
             : type_categories::Simple::value)>::type;

} // namespace traits
} // pie

#endif

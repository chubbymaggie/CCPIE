#ifndef __TYPES_HPP__

#define __TYPES_HPP__ 1

#include <type_traits>

#include <boost/integer.hpp>

namespace types {
  constexpr auto MAX_FEATURE_COUNT = 63;
  constexpr auto CLAUSE_SIZE_LIMIT = 7;

  namespace {
    constexpr auto MAX_FEATURE_VARIABLES_COUNT = MAX_FEATURE_COUNT * 2;
  }

  typedef boost::uint_t<16>::fast test_id_t;
  typedef boost::uint_t<16>::fast clause_id_t;
  typedef boost::int_max_value_t<MAX_FEATURE_VARIABLES_COUNT>::fast feature_id_t;

  template <template <typename...> class C, typename...Ts>
  std::true_type is_base_of_template_impl(const C<Ts...>*);

  template <template <typename...> class C>
  std::false_type is_base_of_template_impl(...);

  template <typename T, template <typename...> class C>
  using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));

}

#endif

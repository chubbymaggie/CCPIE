#ifndef __PIE_UTILITY_H__
#define __PIE_UTILITY_H__

#include <tuple>
#include <utility>

namespace pie {
namespace util {

/* From gcc/libstdc++-v3/include/experimental/tuple */

template <typename _Fn, typename _Tuple, std::size_t... _Idx>
constexpr decltype(auto)
__apply_impl(_Fn && f, _Tuple && t, std::index_sequence<_Idx...>) {
  return std::forward<_Fn>(f)(std::get<_Idx>(std::forward<_Tuple>(t))...);
}

template <typename _Fn, typename _Tuple>
constexpr decltype(auto) apply(_Fn && f, _Tuple && t) {
  using _Indices =
      std::make_index_sequence<std::tuple_size<std::decay_t<_Tuple>>::value>;
  return __apply_impl(
      std::forward<_Fn>(f), std::forward<_Tuple>(t), _Indices{});
}

} // namespace util
} // namespace pie

#endif

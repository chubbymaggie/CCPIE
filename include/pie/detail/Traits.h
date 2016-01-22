#ifndef __PIE_DETAIL_TRAITS_H__
#define __PIE_DETAIL_TRAITS_H__

namespace detail {

namespace __do_not_use_this_namespace__ {

enum class enabler {};

} // namespace __do_not_use_this_namespace__

// --- all
template <typename... Conditions>
struct all;

template <typename Condition>
struct all<Condition> {
  static const bool value = Condition::value;
};

template <typename Condition_1, typename... Conditions>
struct all<Condition_1, Conditions...> {
  static const bool value = Condition_1::value && all<Conditions...>::value;
};

// --- any
template <typename... Conditions>
struct any;

template <typename Condition>
struct any<Condition> {
  static const bool value = Condition::value;
};

template <typename Condition_1, typename... Conditions>
struct any<Condition_1, Conditions...> {
  static const bool value =
      (Condition_1::value) ? true : any<Conditions...>::value;
};

} // namespace detail
#endif

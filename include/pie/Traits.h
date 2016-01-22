#ifndef __PIE_TRAITS_H__
#define __PIE_TRAITS_H__

#include <type_traits>

#include "pie/detail/Traits.h"

namespace pie {

template <typename... Conditions>
using EnableIfAll =
    typename std::enable_if_t<detail::all<Conditions...>::value,
                              detail::__do_not_use_this_namespace__::enabler>;

template <typename... Conditions>
using DisableIfAll =
    typename std::enable_if_t<!detail::all<Conditions...>::value,
                              detail::__do_not_use_this_namespace__::enabler>;

template <typename... Conditions>
using EnableIfAny =
    typename std::enable_if_t<detail::any<Conditions...>::value,
                              detail::__do_not_use_this_namespace__::enabler>;

template <typename... Conditions>
using DisableIfAny =
    typename std::enable_if_t<!detail::any<Conditions...>::value,
                              detail::__do_not_use_this_namespace__::enabler>;

} // namespace pie

#endif

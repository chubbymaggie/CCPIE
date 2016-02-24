#ifndef __PIE_DETAIL_PIENGINE_H__
#define __PIE_DETAIL_PIENGINE_H__

#include <functional>
#include <tuple>

#include <boost/optional.hpp>

namespace pie {
namespace detail {

template <typename ResT, typename... ArgT>
boost::optional<ResT>
callExceptionSafe(const std::function<ResT(const ArgT &...)> &,
                  const ArgT &...);

template <typename ResT, typename... ArgT>
boost::optional<ResT> callExceptionSafe(const std::function<ResT(ArgT &...)> &,
                                        const std::tuple<ArgT...> &);

template <typename ResT, typename... ArgT>
boost::optional<ResT>
callExceptionSafe(const std::function<ResT(const std::tuple<ArgT...> &)> &,
                  const ArgT &...);

} // namespace detail
} // namespace pie

#include "pie/detail/PIEngine.hpp"

#endif

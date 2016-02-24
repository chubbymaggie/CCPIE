#ifndef __PIE_DETAIL_PIENGINE_HPP__
#define __PIE_DETAIL_PIENGINE_HPP__

#include <functional>
#include <tuple>

#include <boost/optional.hpp>

#include "pie/Utility.h"
/* Once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69939
 * is fixed, we can replace the above by:
 * #include <experimental/tuple>
*/

namespace pie {
namespace detail {

template <typename ResT, typename... ArgT>
boost::optional<ResT>
callExceptionSafe(const std::function<ResT(const ArgT &...)> & f,
                  const ArgT &... a) {
  boost::optional<ResT> r;
  try {
    r = f(a...);
  } catch (...) {
  }
  return r;
}

template <typename ResT, typename... ArgT>
boost::optional<ResT>
callExceptionSafe(const std::function<ResT(const ArgT &...)> & f,
                  const std::tuple<ArgT...> & a) {
  boost::optional<ResT> r;
  try {
    r = pie::util::apply(f, a);
  } catch (...) {
  }
  return r;
}

template <typename ResT, typename... ArgT>
boost::optional<ResT>
callExceptionSafe(const std::function<ResT(const std::tuple<ArgT...> &)> & f,
                  const ArgT &... a) {
  boost::optional<ResT> r;
  try {
    r = f(std::make_tuple(a...));
  } catch (...) {
  }
  return r;
}

} // namespace detail
} // namespace pie

#endif

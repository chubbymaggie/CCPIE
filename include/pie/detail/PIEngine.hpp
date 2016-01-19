#ifndef __PIE_DETAIL_PIENGINE_HPP__
#define __PIE_DETAIL_PIENGINE_HPP__

#include <functional>

#include <boost/optional.hpp>

namespace pie {
namespace detail {

template <typename ArgT, typename ResT>
boost::optional<ResT> callExceptionSafe(const std::function<ResT(ArgT)> & f,
                                        const ArgT & a) {
  boost::optional<ResT> r;
  try {
    r = f(a);
  } catch (...) {
  }
  return r;
}

} // namespace detail
} // namespace pie

#endif

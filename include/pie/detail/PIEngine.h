#ifndef __PIE_DETAIL_PIENGINE_H__
#define __PIE_DETAIL_PIENGINE_H__

#include <functional>

#include <boost/optional.hpp>

namespace pie {
namespace detail {

template <typename ArgT, typename ResT>
boost::optional<ResT> callExceptionSafe(const std::function<ResT(ArgT)> &,
                                        const ArgT &);

} // namespace detail
} // namespace pie

#include "pie/detail/PIEngine.hpp"

#endif

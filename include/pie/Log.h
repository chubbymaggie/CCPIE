#ifndef __PIE_LOG_H__
#define __PIE_LOG_H__

#include <string>
#include <type_traits>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

#include "pie/Traits.h"

typedef boost::log::sources::severity_logger<
    boost::log::trivial::severity_level> pie_logger_t;

BOOST_LOG_GLOBAL_LOGGER(pie_logger, pie_logger_t);

/* clang-format off */

#if PIE_TRACE_LOGGING == 1
  #undef PIE_DEBUG_LOGGING
  #define PIE_DEBUG_LOGGING 1
#else
  #undef PIE_TRACE_LOGGING
  #define PIE_TRACE_LOGGING 0
#endif

#if PIE_DEBUG_LOGGING == 1
  #undef PIE_INFO_LOGGING
  #define PIE_INFO_LOGGING 1
#else
  #undef PIE_DEBUG_LOGGING
  #define PIE_DEBUG_LOGGING 0
#endif

#if PIE_INFO_LOGGING == 1
  #undef PIE_WARNING_LOGGING
  #define PIE_WARNING_LOGGING 1
#else
  #undef PIE_INFO_LOGGING
  #define PIE_INFO_LOGGING 0
#endif

#if PIE_WARNING_LOGGING == 1
  #undef PIE_ERROR_LOGGING
  #define PIE_ERROR_LOGGING 1
#else
  #undef PIE_WARNING_LOGGING
  #define PIE_WARNING_LOGGING 0
#endif

#if PIE_ERROR_LOGGING == 1
  #undef PIE_FATAL_LOGGING
  #define PIE_FATAL_LOGGING 1
#else
  #undef PIE_ERROR_LOGGING
  #define PIE_ERROR_LOGGING 0
#endif

#if PIE_FATAL_LOGGING != 1
  #undef PIE_FATAL_LOGGING
  #define PIE_FATAL_LOGGING 0
#endif

#define TRACE                                                                  \
  if (!PIE_TRACE_LOGGING) {}                                                   \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::trace)

#define DEBUG                                                                  \
  if (!PIE_DEBUG_LOGGING) {}                                                   \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::debug)

#define INFO                                                                   \
  if (!PIE_INFO_LOGGING) {}                                                    \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::info)

#define WARNING                                                                \
  if (!PIE_WARNING_LOGGING) {}                                                 \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::warning)

#define ERROR                                                                  \
  if (!PIE_ERROR_LOGGING) {}                                                   \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::error)

#define fatal                                                                  \
  if (!PIE_FATAL_LOGGING) {}                                                   \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::fatal)

/* clang-format on */

namespace pie {

inline std::string indent(unsigned i) { return std::string(i, ' '); }

/* TODO: The following definitions trigger clang bug.
 *       Implement a workaround? Dummy template param? */

template <typename T,
          template <typename...> typename C,
          EnableIfAny<std::is_integral<T>>...>
inline boost::log::formatting_ostream &
operator<<(boost::log::formatting_ostream & o, const C<T> & v) {
  o << "[";
  if (v.size()) {
    auto it = v.cbegin();
    o << " " << static_cast<int_fast64_t>(*it);
    for (++it; it != v.cend(); ++it)
      o << ", " << static_cast<int_fast64_t>(*it);
  }
  return o << " ]";
}

template <typename T,
          template <typename...> typename C,
          DisableIfAny<std::is_integral<T>>...>
inline boost::log::formatting_ostream &
operator<<(boost::log::formatting_ostream & o, const C<T> & v) {
  o << "[";
  if (v.size()) {
    auto it = v.cbegin();
    o << " " << *it;
    for (++it; it != v.cend(); ++it) o << ", " << *it;
  }
  return o << " ]";
}

} // namespace pie

#endif

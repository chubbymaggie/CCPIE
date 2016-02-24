#ifndef __PIE_LOG_H__
#define __PIE_LOG_H__

#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>

#include "pie/detail/Log.h"

typedef boost::log::sources::severity_logger<
    boost::log::trivial::severity_level>
    pie_logger_t;

BOOST_LOG_GLOBAL_LOGGER(pie_logger, pie_logger_t)

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

#define FATAL                                                                  \
  if (!PIE_FATAL_LOGGING) {}                                                   \
  else BOOST_LOG_SEV(pie_logger::get(), boost::log::trivial::fatal)

/* clang-format on */

namespace pie {

inline std::string indent(unsigned i) { return std::string(i, ' '); }

} // namespace pie

namespace boost {
namespace log {

template <typename A, typename B>
inline formatting_ostream & operator<<(formatting_ostream & o,
                                       const std::pair<A, B> & p) {
  return o << "( " << p.first << ", " << p.second << " )";
}

template <typename... Ts>
inline formatting_ostream & operator<<(formatting_ostream & o,
                                       const std::tuple<Ts...> & t) {
  return pie::detail::writeTupleToStream(o, t, std::index_sequence_for<Ts...>{});
}

template <typename T>
inline formatting_ostream & operator<<(formatting_ostream & o,
                                       const std::vector<T> & v) {
  return pie::detail::writeContainerToStream(o, v);
}

template <typename T>
inline formatting_ostream & operator<<(formatting_ostream & o,
                                       const std::unordered_set<T> & s) {
  return pie::detail::writeContainerToStream(o, s);
}

} // namespace log
} // namespace boost

#endif

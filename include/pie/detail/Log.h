#ifndef __PIE_DETAIL_LOG_H__
#define __PIE_DETAIL_LOG_H__

#include <utility>

#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace pie {
namespace detail {

// from http://en.cppreference.com/w/cpp/utility/integer_sequence

template <typename Tuple, std::size_t... Is>
inline boost::log::formatting_ostream &
writeTupleToStream(boost::log::formatting_ostream & o,
                   const Tuple & t,
                   std::index_sequence<Is...>) {
  using swallow = int[]; // guaranties left to right order
  o << "(";
  (void)swallow{0, (void(o << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...};
  return o << ")";
}

template <typename T,
          template <typename...> class C,
          typename I = typename std::
              conditional<std::is_integral<T>::value, int_fast64_t, T>::type>
inline boost::log::formatting_ostream &
writeContainerToStream(boost::log::formatting_ostream & o, const C<T> & c) {
  BOOST_CONCEPT_ASSERT((boost::Container<C<T>>));

  o << "[";
  if (c.size()) {
    auto it = c.cbegin();
    o << " " << static_cast<I>(*it);
    for (++it; it != c.cend(); ++it) o << ", " << static_cast<I>(*it);
  }
  return o << " ]";
}

} // namespace detail
} // detail pie

#endif

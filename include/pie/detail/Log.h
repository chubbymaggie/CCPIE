#ifndef __PIE_DETAIL_LOG_H__
#define __PIE_DETAIL_LOG_H__

namespace pie {
namespace detail {

template <typename T,
          template <typename...> class C,
          typename I = typename std::
              conditional<std::is_integral<T>::value, int_fast64_t, T>::type>
inline boost::log::formatting_ostream &
writeToStream(boost::log::formatting_ostream & o, const C<T> & v) {
  BOOST_CONCEPT_ASSERT((boost::Container<C<T>>));

  o << "[";
  if (v.size()) {
    auto it = v.cbegin();
    o << " " << static_cast<I>(*it);
    for (++it; it != v.cend(); ++it) o << ", " << static_cast<I>(*it);
  }
  return o << " ]";
}

} // namespace detail
} // detail pie

#endif

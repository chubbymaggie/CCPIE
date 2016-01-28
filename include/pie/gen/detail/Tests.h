#ifndef __PIE_GEN_DETAILS_TESTS_H__
#define __PIE_GEN_DETAILS_TESTS_H__

#include <type_traits>

namespace pie {
namespace gen {
namespace detail {

template <typename RandGen, typename Dist, typename T>
class TestsBase {
public:
  TestsBase(RandGen & r) : rnd_gen(r) {}
  virtual T operator[](Dist &) = 0;

protected:
  RandGen & rnd_gen;
};

template <
    typename RandGen,
    typename Dist,
    typename T,
    typename = /* Tag for integral data types */
    typename std::integral_constant<bool, std::is_integral<T>::value>::type>
class Tests;

} // namespace detail
} // namespace gen
} // namespace pie

#endif

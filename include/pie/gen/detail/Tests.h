#ifndef __PIE_GEN_DETAIL_TESTS_H__
#define __PIE_GEN_DETAIL_TESTS_H__

#include "pie/Traits.h"

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

} // namespace detail

template <typename RandGen,
          typename Dist,
          typename T,
          typename = typename pie::traits::TypePropertyCodeSum<T>>
class Tests;

} // namespace gen
} // namespace pie

#endif

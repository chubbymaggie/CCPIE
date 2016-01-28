#ifndef __PIE_GEN_TESTS_H__
#define __PIE_GEN_TESTS_H__

#include "pie/gen/tests/Int.hpp"

namespace pie {
namespace gen {

template <template <typename...> class BackSequence,
          typename T,
          typename Dist,
          typename RandGen>
BackSequence<T> testSequence(unsigned count, Dist & dist, RandGen & rand) {
  BackSequence<T> result;
  Tests<T, Dist, RandGen> test_gen(dist);
  for (unsigned i = 0; i < count; ++i) result.push_back(test_gen[rand]);
  return result;
}

} // namespace gen
} // namespace pie

#endif

#ifndef __PIE_GEN_TESTS_H__
#define __PIE_GEN_TESTS_H__

#include "pie/gen/tests/Integers.hpp"
#include "pie/gen/tests/String.hpp"

#include "pie/gen/tests/Pair.hpp"
#include "pie/gen/tests/Tuple.hpp"

#include "pie/gen/tests/BackPushers.hpp"
#include "pie/gen/tests/Inserters.hpp"

namespace pie {
namespace gen {

template <typename T, typename RandGen, typename Dist>
std::vector<T> testVector(unsigned count, Dist & dist, RandGen & rand) {
  std::vector<T> result;
  Tests<RandGen, Dist, T> test_gen(rand);
  for (unsigned i = 0; i < count; ++i) result.push_back(test_gen[dist]);
  return result;
}

} // namespace gen
} // namespace pie

#endif

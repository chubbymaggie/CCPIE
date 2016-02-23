#ifndef __PIE_GEN_TESTS_H__
#define __PIE_GEN_TESTS_H__

#include <boost/concept/assert.hpp>

#include "pie/gen/tests/Integers.hpp"
#include "pie/gen/tests/String.hpp"

#include "pie/gen/tests/Pair.hpp"
#include "pie/gen/tests/Tuple.hpp"

#include "pie/gen/tests/BackPushers.hpp"
#include "pie/gen/tests/Inserters.hpp"

namespace pie {
namespace gen {

template <template <typename...> class BackSequence,
          typename T,
          typename RandGen,
          typename Dist>
BackSequence<T> testSequence(unsigned count, Dist & dist, RandGen & rand) {
  BOOST_CONCEPT_ASSERT((boost::BackInsertionSequence<BackSequence<int>>));

  BackSequence<T> result;
  Tests<RandGen, Dist, T> test_gen(rand);
  for (unsigned i = 0; i < count; ++i) result.push_back(test_gen[dist]);
  return result;
}

} // namespace gen
} // namespace pie

#endif

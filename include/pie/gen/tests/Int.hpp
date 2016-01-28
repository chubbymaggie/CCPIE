#ifndef __PIE_GEN_TESTS_INT_H__
#define __PIE_GEN_TESTS_INT_H__

#include <limits>
#include <type_traits>

#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist, typename Integer>
class Tests<RandGen, Dist, Integer, std::true_type>
    : public detail::TestsBase<RandGen, Dist, Integer> {
public:
  using detail::TestsBase<RandGen, Dist, Integer>::TestsBase;

  Integer operator[](Dist & d) {
    auto x = static_cast<Integer>(d(this->rnd_gen));
    while (x < std::numeric_limits<Integer>::min() ||
           x > std::numeric_limits<Integer>::max()) {
      x = d(this->rnd_gen);
    }

    return x;
  }
};

} // namespace gen
} // namespace pie

#endif

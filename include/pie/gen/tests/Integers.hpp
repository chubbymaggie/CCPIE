#ifndef __PIE_GEN_TESTS_INTEGERS_HPP__
#define __PIE_GEN_TESTS_INTEGERS_HPP__

#include <limits>
#include <type_traits>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist, typename Integer>
class Tests<RandGen, Dist, Integer, pie::traits::type_categories::Integer>
    : public detail::TestsBase<RandGen, Dist, Integer> {
public:
  using detail::TestsBase<RandGen, Dist, Integer>::TestsBase;

  Integer operator[](Dist & d) {
    double x = d(this->rnd_gen);

    double low0 = d.min(), high0 = d.max();
    double low1 = std::numeric_limits<Integer>::min(),
           high1 = std::numeric_limits<Integer>::max();
    double low = std::max(low0, low1), high = std::min(high0, high1);

    while (x < low || x > high) x = d(this->rnd_gen);

    return x;
  }
};

} // namespace gen
} // namespace pie

#endif

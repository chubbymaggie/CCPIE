#ifndef __PIE_GEN_TESTS_PAIR_HPP__
#define __PIE_GEN_TESTS_PAIR_HPP__

#include <utility>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist, typename A, typename B>
class Tests<RandGen,
            Dist,
            std::pair<A, B>,
            pie::traits::type_categories::Simple>
    : public detail::TestsBase<RandGen, Dist, std::pair<A, B>> {
public:
  using detail::TestsBase<RandGen, Dist, std::pair<A, B>>::TestsBase;

  std::pair<A, B> operator[](Dist & d) {
    return std::make_pair(Tests<RandGen, Dist, A>(this->rnd_gen)[d],
                          Tests<RandGen, Dist, B>(this->rnd_gen)[d]);
  }
};

} // namespace gen
} // namespace pie

#endif

#ifndef __PIE_GEN_TESTS_VECTOR_H__
#define __PIE_GEN_TESTS_VECTOR_H__

#include <vector>

#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist, typename A>
class Tests<RandGen, Dist, std::vector<A>>
    : public detail::TestsBase<RandGen, Dist, std::vector<A>> {
public:
  using detail::TestsBase<RandGen, Dist, std::vector<A>>::TestsBase;

  std::vector<A> operator[](Dist & d) {
    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];

    std::vector<A> v;
    for (size_t i = 0; i < size; ++size)
      v.push_back(Tests<RandGen, Dist, A>(this->rnd_gen)[d]);

    return v;
  }
};

} // namespace gen
} // namespace pie

#endif

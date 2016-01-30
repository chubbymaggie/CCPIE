#ifndef __PIE_GEN_TESTS_BACKPUSHERS_HPP__
#define __PIE_GEN_TESTS_BACKPUSHERS_HPP__

#include <vector>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen,
          typename Dist,
          typename A,
          template <typename...> class T>
class Tests<RandGen, Dist, T<A>, pie::traits::type_categories::BackPusher>
    : public detail::TestsBase<RandGen, Dist, T<A>> {
public:
  using detail::TestsBase<RandGen, Dist, T<A>>::TestsBase;

  T<A> operator[](Dist & d) {
    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];

    T<A> v;
    for (size_t i = 0; i < size; ++i)
      v.push_back(Tests<RandGen, Dist, A>(this->rnd_gen)[d]);

    return v;
  }
};

} // namespace gen
} // namespace pie

#endif

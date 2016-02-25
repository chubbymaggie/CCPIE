#ifndef __PIE_GEN_TESTS_INSERTERS_HPP__
#define __PIE_GEN_TESTS_INSERTERS_HPP__

#include <utility>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen,
          typename Dist,
          typename A,
          typename B,
          template <typename...> class T>
class Tests<RandGen, Dist, T<A, B>, pie::traits::type_categories::PairInserter>
    : public detail::TestsBase<RandGen, Dist, T<A, B>> {
public:
  using detail::TestsBase<RandGen, Dist, T<A, B>>::TestsBase;

  T<A, B> operator[](Dist & d) {
    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];

    T<A, B> t;
    for (size_t i = 0; i < size; ++i)
      t.insert(Tests<RandGen, Dist, std::pair<A, B>>(this->rnd_gen)[d]);
    return t;
  }
};

template <typename RandGen,
          typename Dist,
          typename A,
          template <typename...> class T>
class Tests<RandGen, Dist, T<A>, pie::traits::type_categories::Inserter>
    : public detail::TestsBase<RandGen, Dist, T<A>> {
public:
  using detail::TestsBase<RandGen, Dist, T<A>>::TestsBase;

  T<A> operator[](Dist & d) {
    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];

    T<A> t;
    for (size_t i = 0; i < size; ++i)
      t.insert(Tests<RandGen, Dist, A>(this->rnd_gen)[d]);
    return t;
  }
};

} // namespace gen
} // namespace pie

#endif

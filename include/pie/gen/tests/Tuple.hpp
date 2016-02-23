#ifndef __PIE_GEN_TESTS_TUPLE_HPP__
#define __PIE_GEN_TESTS_TUPLE_HPP__

#include <tuple>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist, typename... Ts>
class Tests<RandGen,
            Dist,
            std::tuple<Ts...>,
            pie::traits::type_categories::Simple>
    : public detail::TestsBase<RandGen, Dist, std::tuple<Ts...>> {
public:
  using detail::TestsBase<RandGen, Dist, std::tuple<Ts...>>::TestsBase;

  std::tuple<Ts...> operator[](Dist & d) {
    return std::make_tuple(Tests<RandGen, Dist, Ts>(this->rnd_gen)[d]...);
  }
};

} // namespace gen
} // namespace pie

#endif

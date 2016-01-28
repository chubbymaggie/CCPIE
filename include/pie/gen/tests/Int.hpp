#ifndef __PIE_GEN_TESTS_INT_H__
#define __PIE_GEN_TESTS_INT_H__

#include <type_traits>

#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename Integer,
          typename Dist,
          typename RandGen,
          typename =
              typename std::enable_if<std::is_integral<Integer>::value>::type>
class Tests : public detail::TestsBase<Integer, Dist, RandGen> {
public:
  using detail::TestsBase<Integer, Dist, RandGen>::TestsBase;
  Integer operator[](RandGen & r) { return this->dist(r); }
};

} // namespace gen
} // namespace pie

#endif

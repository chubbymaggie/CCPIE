#ifndef __PIE_GEN_DETAILS_TESTS_H__
#define __PIE_GEN_DETAILS_TESTS_H__

namespace pie {
namespace gen {
namespace detail {

template <typename T, typename Dist, typename RandGen>
class TestsBase {
public:
  TestsBase(Dist & d) : dist(d) {}
  virtual T operator[](RandGen &) = 0;

protected:
  Dist & dist;
};

} // namespace detail
} // namespace gen
} // namespace pie

#endif

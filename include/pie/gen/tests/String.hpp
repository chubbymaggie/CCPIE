#ifndef __PIE_GEN_TESTS_STRING_H__
#define __PIE_GEN_TESTS_STRING_H__

#include <string>

#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist>
class Tests<RandGen, Dist, std::string>
    : public detail::TestsBase<RandGen, Dist, std::string> {
public:
  using detail::TestsBase<RandGen, Dist, std::string>::TestsBase;

  std::string operator[](Dist & d) {
    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];

    std::string s;
    for (size_t i = 0; i < size; ++size)
      s += Tests<RandGen, Dist, char>(this->rnd_gen)[d];

    return s;
  }
};

} // namespace gen
} // namespace pie

#endif

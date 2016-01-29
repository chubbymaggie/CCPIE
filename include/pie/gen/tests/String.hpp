#ifndef __PIE_GEN_TESTS_STRING_HPP__
#define __PIE_GEN_TESTS_STRING_HPP__

#include <algorithm>
#include <random>
#include <string>

#include "pie/Traits.h"
#include "pie/gen/detail/Tests.h"

namespace pie {
namespace gen {

template <typename RandGen, typename Dist>
class Tests<RandGen, Dist, std::string, pie::traits::type_categories::Simple>
    : public detail::TestsBase<RandGen, Dist, std::string> {
public:
  using detail::TestsBase<RandGen, Dist, std::string>::TestsBase;

  std::string operator[](Dist & d) {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "0123456789!@#$%^&*()-=_+~`"
                                   "[]{}|:;<>?,.\'\"\\";

    size_t size = Tests<RandGen, Dist, size_t>(this->rnd_gen)[d];
    std::uniform_int_distribution<> dist(
        0, sizeof(alphabet) / sizeof(*alphabet) - 2);
    std::string s;
    s.reserve(size);

    std::generate_n(std::back_inserter(s), size, [&]() {
      return alphabet[dist(this->rnd_gen)];
    });

    return s;
  }
};

} // namespace gen
} // namespace pie

#endif

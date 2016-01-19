#ifndef __PIE_GEN_FEATURES_H__
#define __PIE_GEN_FEATURES_H__

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "pie/gen/detail/Features.h"

namespace pie {
namespace gen {

constexpr static auto MUTUALLY_EXCLUSIVE_ONLY = true;

template <typename Arith,
          class =
              typename std::enable_if<std::is_arithmetic<Arith>::value>::type>
class Features : public detail::FeaturesBase<Arith> {
public:
  using detail::FeaturesBase<Arith>::FeaturesBase;

  template <
      typename Formatter,
      typename FeatureCollection = std::vector<
          std::pair<std::function<bool(Arith)>, typename Formatter::FormatT>>>
  FeatureCollection operator[](Formatter && formatter) const {
    auto a = formatter.variable(this->args[0]);
    auto b = formatter.constant("0");

    FeatureCollection res = {
        {[](Arith a) { return a > 0; }, formatter.GT(a, b)},
        {[](Arith a) { return a == 0; }, formatter.EQ(a, b)},
    };

    if (!MUTUALLY_EXCLUSIVE_ONLY) {
      res.push_back({[](Arith a) { return a < 0; }, formatter.LT(a, b)});
    }

    return res;
  }
};

} // namespace gen
} // namespace pie

#endif

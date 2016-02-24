#ifndef __PIE_GEN_FEATURES_INTEGERS_H__
#define __PIE_GEN_FEATURES_INTEGERS_H__

#include "pie/Traits.h"
#include "pie/gen/detail/Features.h"

namespace pie {
namespace gen {

template <typename Formatter, typename Integer>
class Features<Formatter, Integer, pie::traits::type_categories::Integer>
    : public detail::FeaturesBase<Formatter, Integer> {
  using __base = detail::FeaturesBase<Formatter, Integer>;

public:
  using detail::FeaturesBase<Formatter, Integer>::FeaturesBase;

  typename __base::FeatureCollection
  operator[](bool mutually_exclusive_only) const {
    Formatter formatter;
    auto a = formatter.variable(this->args.at(0));
    auto b = formatter.constant("0");

    typename __base::FeatureCollection res = {
        {[](const Integer & a) { return a > 0; }, formatter.GT(a, b)},
        {[](const Integer & a) { return a == 0; }, formatter.EQ(a, b)},
    };

    if (!mutually_exclusive_only) {
      res.push_back(
          {[](const Integer & a) { return a < 0; }, formatter.LT(a, b)});
    }

    return res;
  }
};

} // namespace gen
} // namespace pie

#endif

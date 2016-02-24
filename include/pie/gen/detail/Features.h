#ifndef __PIE_GEN_DETAIL_FEATURES_H__
#define __PIE_GEN_DETAIL_FEATURES_H__

#include <string>
#include <utility>
#include <vector>

#include "pie/Traits.h"

namespace pie {
namespace gen {
namespace detail {

template <typename Formatter, typename... ArgT>
class FeaturesBase {
public:
  using FeatureCollection =
      std::vector<std::pair<std::function<bool(const ArgT &...)>,
                            typename Formatter::FormatT>>;

  FeaturesBase(std::array<std::string, sizeof...(ArgT)> names) : args(names) {}

  virtual FeatureCollection operator[](bool) const = 0;

protected:
  std::array<std::string, sizeof...(ArgT)> args;
};

} // namespace detail

template <typename Formatter,
          typename T,
          typename = typename pie::traits::TypePropertyCodeSum<T>>
class Features;

} // namespace gen
} // namespace pie

#endif

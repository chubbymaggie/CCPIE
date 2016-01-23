#ifndef __PIE_GEN_DETAIL_FEATURES_H__
#define __PIE_GEN_DETAIL_FEATURES_H__

#include <string>
#include <vector>

namespace pie {
namespace gen {
namespace detail {

template <typename T>
class FeaturesBase {
public:
  FeaturesBase(std::vector<std::string> names) : args(names) {}

  /* TODO: Not enforcing this in derived classes, not pure virtual */
  template <typename Formatter, typename FeatureCollection>
  FeatureCollection operator[](Formatter && formatter) const {
    throw std::runtime_error("Undefined features!");
  }

protected:
  std::vector<std::string> args;
};

} // namespace detail
} // namespace gen
} // namespace pie

#endif

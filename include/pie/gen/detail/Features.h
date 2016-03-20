#ifndef __PIE_GEN_DETAIL_FEATURES_H__
#define __PIE_GEN_DETAIL_FEATURES_H__

#include "pie/Traits.h"
#include "pie/config.h"

namespace pie {
namespace gen {
namespace detail {

template <typename Formatter, typename... ArgT>
class FeaturesBase {
public:
  using FeatureCollection = RawFeatureVectorType<Formatter, ArgT...>;

  virtual ~FeaturesBase() = default;
  FeaturesBase(NamesArrayFor<ArgT...> names) : args(names) {}

  virtual FeatureCollection operator[](bool) const = 0;

protected:
  NamesArrayFor<ArgT...> args;
};

} // namespace detail

template <typename Formatter,
          typename T,
          typename = typename pie::traits::TypePropertyCodeSum<T>>
class Features;

} // namespace gen
} // namespace pie

#endif

#ifndef __PIE_PIENGINE_H__
#define __PIE_PIENGINE_H__

#include <functional>
#include <tuple>
#include <utility>

#include <boost/optional.hpp>

namespace pie {

template <typename ResT, typename Formatter, typename... ArgT>
class PIEngine {
public:
  PIEngine(const std::function<ResT(const ArgT &...)> &,
           const RawPostConditionType<ResT, Formatter, ArgT...> &,
           const TupledVector<ArgT...> &,
           const RawFeatureVectorType<Formatter, ArgT...> &);

  template <typename Learner>
  std::pair<bfl::LearnerStatus, typename Formatter::T> inferCNF() const;

  PIEngine & add_test(const ArgT &&...);
  PIEngine & add_test(const std::tuple<ArgT...> &);

  PIEngine & add_feature(const FeatureType<Formatter, ArgT...> &);
  PIEngine & add_feature(const RawFeatureType<Formatter, ArgT...> &);

protected:
  const SafeTupledArgTFunction<ResT, ArgT...> func;
  const PostConditionType<ResT, Formatter, ArgT...> post;

  TupledVector<ArgT...> tests;
  FeatureVectorType<Formatter, ArgT...> features;

  unsigned int orig_tests, orig_features;
};

} // namespace pie

#include "pie/PIEngine.hpp"

#endif

#ifndef __PIE_PIENGINE_H__
#define __PIE_PIENGINE_H__

#include <functional>
#include <utility>

#include <boost/optional.hpp>

namespace pie {

template <typename ArgT, typename ResT, typename FormatT>
class PIEngine {
public:
  using FeatureT =
      std::pair<std::function<boost::optional<bool>(ArgT)>, FormatT>;
  using PostT =
      std::pair<std::function<bool(ArgT, boost::optional<ResT>)>, FormatT>;

  PIEngine(std::vector<std::pair<std::function<bool(ArgT)>, FormatT>> &&,
           std::function<ResT(ArgT)> &&,
           PostT &&,
           std::vector<ArgT> &&);

  template <typename Learner, typename Formatter>
  FormatT inferCNF() const;

  PIEngine<ArgT, ResT, FormatT> & add_test(ArgT &&);

  PIEngine<ArgT, ResT, FormatT> &
  add_feature(std::pair<std::function<bool(ArgT)>, FormatT> &&);

protected:
  std::vector<FeatureT> features;
  const std::function<ResT(ArgT)> func;
  const PostT post;
  std::vector<ArgT> tests;
  int orig_tests, orig_features;
};
}

#include "PIEngine.hpp"

#endif

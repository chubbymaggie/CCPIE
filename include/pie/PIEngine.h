#ifndef __PIE_PIENGINE_H__
#define __PIE_PIENGINE_H__

#include <functional>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/optional.hpp>

namespace pie {

template <typename ResT, typename Formatter, typename... ArgT>
class PIEngine {
public:
  using FormatT = typename Formatter::FormatT;

  using FeatureT = std::pair<
      std::function<boost::optional<bool>(const std::tuple<ArgT...> &)>,
      FormatT>;

  using PostT = std::pair<std::function<bool(const std::tuple<ArgT...> &,
                                             const boost::optional<ResT> &)>,
                          FormatT>;

  /* TODO: Make an overloaded constructor for the case when ArgT... is only one
   * type. Then the tests can just be of type ArgT, instead of tuple<ArgT...>*/
  PIEngine(const std::function<ResT(const ArgT &...)> &,
           const std::pair<std::function<bool(const boost::optional<ResT> &,
                                              const ArgT &...)>,
                           FormatT> &,
           const std::vector<std::tuple<ArgT...>> &,
           const std::vector<
               std::pair<std::function<bool(const ArgT &...)>, FormatT>> &);

  template <typename Learner>
  std::pair<bfl::LearnerStatus, FormatT> inferCNF() const;

  PIEngine & add_test(const ArgT &&...);
  PIEngine & add_test(const std::tuple<ArgT...> &);

  PIEngine &
  add_feature(const std::pair<std::function<bool(const ArgT &...)>, FormatT> &);

protected:
  const std::function<boost::optional<ResT>(const std::tuple<ArgT...> &)> func;
  const PostT post;
  std::vector<std::tuple<ArgT...>> tests;
  std::vector<FeatureT> features;
  unsigned int orig_tests, orig_features;
};

} // namespace pie

#include "PIEngine.hpp"

#endif

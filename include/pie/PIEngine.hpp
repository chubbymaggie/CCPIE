#ifndef __PIE_PIENGINE_HPP__
#define __PIE_PIENGINE_HPP__

#include <numeric>
#include <utility>
#include <vector>

#include "pie/config.h"

#include "pie/bfl/ILearner.h"

#include "pie/detail/PIEngine.h"

namespace pie {

template <typename ArgT, typename ResT, class FormatT>
template <typename Learner, typename Formatter>
FormatT PIEngine<ArgT, ResT, FormatT>::inferCNF(Formatter && formatter) const {
  Learner learner(features.size());

  for (auto && t : tests) {
    learner += {std::accumulate(features.begin(),
                                features.end(),
                                BitVector(),
                                [&t](BitVector & bv, const FeatureT & f) {
                                  auto b = f.first(t);
                                  bv.push_back(b && *b);
                                  return bv;
                                }),
                0,
                post.first(t, detail::callExceptionSafe(func, t))};
  }
  auto result = learner.learnCNF();

  if (result.first == pie::bfl::FAIL)
    throw std::runtime_error("FAILED");
  else if (result.first == pie::bfl::BAD_FUNCTION) {
    throw std::runtime_error("BAD FUNCTION");
  }

  typename Formatter::FormatCNF format_cnf;
  for (auto && c : result.second) {
    typename Formatter::FormatCNF::value_type nc;
    for (auto && f : c) {
      if (f > 0)
        nc.push_back(features[f - learner.featureIndexOffset()].second);
      else
        nc.push_back(
            formatter.NOT(features[-f - learner.featureIndexOffset()].second));
    }
    format_cnf.push_back(nc);
  }
  return formatter.format(format_cnf);
}

template <typename ArgT, typename ResT, class FormatT>
PIEngine<ArgT, ResT, FormatT> &
PIEngine<ArgT, ResT, FormatT>::add_test(ArgT && t) {
  tests.push_back(t);
  return *this;
}

template <typename ArgT, typename ResT, class FormatT>
PIEngine<ArgT, ResT, FormatT> & PIEngine<ArgT, ResT, FormatT>::add_feature(
    std::pair<std::function<bool(ArgT)>, FormatT> && f) {
  features.push_back( {[f = std::move(f.first)](ArgT a) { return detail::callExceptionSafe(f, a);
}
, std::move(f.second)
});

return *this;
}

template <typename ArgT, typename ResT, class FormatT>
PIEngine<ArgT, ResT, FormatT>::PIEngine(
    std::vector<std::pair<std::function<bool(ArgT)>, FormatT>> && fs,
    std::function<ResT(ArgT)> && f,
    PostT && p,
    std::vector<ArgT> && ts)
    : func(f), post(p), tests(ts), orig_tests(ts.size()),
      orig_features(fs.size()) {

  /* Save exception-safe versions of the features. */
  for (auto fi : fs) {
    features.push_back(
        {[fi = std::move(fi.first)](ArgT a)->boost::optional<bool> {
          return detail::callExceptionSafe(fi, a);
  }
  , std::move(fi.second)
});
}
}

} // namespace pie

#endif

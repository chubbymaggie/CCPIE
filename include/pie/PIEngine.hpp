#ifndef __PIE_PIENGINE_HPP__
#define __PIE_PIENGINE_HPP__

#include <numeric>
#include <utility>
#include <vector>

#include "pie/config.h"

#include "pie/bfl/ILearner.h"

#include "pie/detail/PIEngine.h"

#include "pie/Log.h"

namespace pie {

/* TODO: Assuming that ArgT, ResT and FormatT are printable for debugging */

/* TODO: Cache learner state instead of constructing a new instance for every
 * call to inferCNF */

template <typename ArgT, typename ResT, typename Formatter>
template <typename Learner>
std::pair<bfl::LearnerStatus, typename Formatter::FormatT>
PIEngine<ArgT, ResT, Formatter>::inferCNF() const {
  Learner learner(features.size());

  INFO << "Invoking boolean function learner ...";
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
  Formatter formatter;

  if (result.first != bfl::PASS) return {result.first, formatter.format({{}})};

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

  auto final_cnf = formatter.format(format_cnf);
  INFO << "Result = " << final_cnf;
  return {bfl::PASS, final_cnf};
}

template <typename ArgT, typename ResT, typename Formatter>
PIEngine<ArgT, ResT, Formatter> &
PIEngine<ArgT, ResT, Formatter>::add_test(ArgT && t) {
  INFO << "Added new test: " << t;
  tests.push_back(t);
  return *this;
}

template <typename ArgT, typename ResT, typename Formatter>
PIEngine<ArgT, ResT, Formatter> & PIEngine<ArgT, ResT, Formatter>::add_feature(
    std::pair<std::function<bool(ArgT)>, typename Formatter::FormatT> && f) {
  INFO << "Added new feature: " << f.second;

  /* clang-format off */

  /* TODO: No C++14 support in clang-format yet (3.6). Does a horrible job at
   * the snippet below (as it uses generic lambda-capture initializers). */

  features.push_back({
    [f = std::move(f.first)](ArgT a) { return detail::callExceptionSafe(f, a);},
    std::move(f.second)});


  return *this;
/* clang-format on */
}

template <typename ArgT, typename ResT, typename Formatter>
PIEngine<ArgT, ResT, Formatter>::PIEngine(
    std::vector<std::pair<std::function<bool(ArgT)>,
                          typename Formatter::FormatT>> && fs,
    std::function<ResT(ArgT)> && f,
    PostT && p,
    std::vector<ArgT> && ts)
    : func(f), post(p), tests(ts), orig_tests(ts.size()),
      orig_features(fs.size()) {

  INFO << "New PIE [ " << typeid(ArgT).name() << " -> " << typeid(ResT).name()
       << " ] instance created { " << orig_tests << " T x " << orig_features
       << " F }";

  DEBUG << indent(2) << "Post:";
  DEBUG << indent(4) << p.second;

  DEBUG << indent(2) << "Tests:";
  for (auto && t : tests) DEBUG << indent(4) << ". " << t;

  DEBUG << indent(2) << "Features:";
  {
    auto i = 0;
    for (auto f = fs.cbegin(); f != fs.cend(); ++f, ++i)
      DEBUG << indent(4) << i << ". " << f->second;
  }

  /* clang-format off */

  /* TODO: No C++14 support in clang-format yet (3.6). Does a horrible job at
   * the snippet below (as it uses generic lambda-capture initializers).
   *
   * Save exception-safe versions of the features.
   */
  for (auto fi : fs) {
    features.push_back({
      [fi = std::move(fi.first)](ArgT a)->boost::optional<bool> {
          return detail::callExceptionSafe(fi, a); },
      std::move(fi.second)});
  }
/* clang-format on */
}

} // namespace pie

#endif

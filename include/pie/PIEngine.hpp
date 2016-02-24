#ifndef __PIE_PIENGINE_HPP__
#define __PIE_PIENGINE_HPP__

#include <numeric>
#include <utility>
#include <vector>

/* TODO: Include the following only for Debug builds or when Logging >= INFO */
#include <cxxabi.h>

#include "pie/Log.h"
#include "pie/bfl/ILearner.h"
#include "pie/config.h"
#include "pie/detail/PIEngine.h"

namespace pie {

/* TODO: Assuming that ArgT, ResT and FormatT are printable for debugging */

/* TODO: Cache learner state instead of constructing a new instance for every
 * call to inferCNF */

template <typename ResT, typename Formatter, typename... ArgT>
template <typename Learner>
std::pair<bfl::LearnerStatus, typename Formatter::FormatT>
PIEngine<ResT, Formatter, ArgT...>::inferCNF() const {
  Learner learner(features.size());

  INFO << "Invoking boolean function learner ...";
  for (const auto & t : tests) {
    learner += {std::accumulate(features.begin(),
                                features.end(),
                                BitVector(),
                                [&t](BitVector & bv, const FeatureT & f) {
                                  auto b = f.first(t);
                                  bv.push_back(b && *b);
                                  return bv;
                                }),
                0,
                post.first(t, func(t))};
  }
  auto result = learner.learnCNF();
  Formatter formatter;

  if (result.first != bfl::PASS) return {result.first, formatter.format({{}})};

  typename Formatter::FormatCNF format_cnf;
  for (const auto & c : result.second) {
    typename Formatter::FormatCNF::value_type nc;
    for (const auto & f : c) {
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

template <typename ResT, typename Formatter, typename... ArgT>
PIEngine<ResT, Formatter, ArgT...> &
PIEngine<ResT, Formatter, ArgT...>::add_test(const ArgT &&... t) {
  INFO << "Added new test: " << std::make_tuple(t...);
  tests.push_back(std::make_tuple(t...));
  return *this;
}

template <typename ResT, typename Formatter, typename... ArgT>
PIEngine<ResT, Formatter, ArgT...> &
PIEngine<ResT, Formatter, ArgT...>::add_test(const std::tuple<ArgT...> & t) {
  INFO << "Added new test: " << t;
  tests.push_back(t);
  return *this;
}

template <typename ResT, typename Formatter, typename... ArgT>
PIEngine<ResT, Formatter, ArgT...> &
PIEngine<ResT, Formatter, ArgT...>::add_feature(
    const std::pair<std::function<bool(const ArgT &...)>,
                    typename Formatter::FormatT> & feature) {
  INFO << "Added new feature: " << feature.second;

  /* clang-format off */

  /* TODO: No C++14 support in clang-format yet (3.6). Does a horrible job at
   * the snippet below (as it uses generic lambda-capture initializers). */

  auto & f = feature.first;
  features.push_back({
    [f = feature.first] (const std::tuple<ArgT...> & a) -> boost::optional<bool>
      { return detail::callExceptionSafe(f, a); },
    feature.second});

  return *this;
/* clang-format on */
}

template <typename ResT, typename Formatter, typename... ArgT>
PIEngine<ResT, Formatter, ArgT...>::PIEngine(
    const std::function<ResT(const ArgT &...)> & f,
    const std::pair<
        std::function<bool(const boost::optional<ResT> &, const ArgT &...)>,
        FormatT> & p,
    const std::vector<std::tuple<ArgT...>> & ts,
    const std::vector<
        std::pair<std::function<bool(const ArgT &...)>, FormatT>> & fs)

    : /* clang-format off */
      func([f](const std::tuple<ArgT...> & t) -> boost::optional<ResT> {
        return detail::callExceptionSafe(f, t);
      }),
      post({
        [f = p.first]
        (const std::tuple<ArgT...> & i, const boost::optional<ResT> & r)
        -> bool {
          auto b = detail::callExceptionSafe(
              f,
              std::tuple_cat(std::make_tuple(r), i));
          return *b && b; },
        p.second}),
      /* clang-format on */
      tests(ts), orig_tests(ts.size()), orig_features(fs.size()) {

  char * demangled_name;
  int demangle_status;
  INFO << "New PIE [ "
       << (demangled_name = abi::__cxa_demangle(
               typeid(std::tuple<ArgT...>).name(), 0, 0, &demangle_status))
       << " -> " << typeid(ResT).name() << " ] instance created { "
       << orig_tests << " T x " << orig_features << " F }";
  free(demangled_name);

  DEBUG << indent(2) << "Post:";
  DEBUG << indent(4) << post.second;

  DEBUG << indent(2) << "Features:";
  {
    auto i = 0;
    for (auto f = fs.cbegin(); f != fs.cend(); ++f, ++i)
      DEBUG << indent(4) << i << ". " << f->second;
  }

  DEBUG << indent(2) << "Tests:";
  for (const auto & t : tests) DEBUG << indent(4) << "@ " << t;

  /* clang-format off */

  /* TODO: No C++14 support in clang-format yet (3.6). Does a horrible job at
   * the snippet below (as it uses generic lambda-capture initializers). */

  for (const auto & fi : fs) {
    features.push_back({
      [f = fi.first] (const std::tuple<ArgT...> & a) -> boost::optional<bool> {
        return detail::callExceptionSafe(f, a); },
      fi.second});
  }
/* clang-format on */
}

} // namespace pie

#endif

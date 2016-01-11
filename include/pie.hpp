#ifndef __PIE_HPP__

#define __PIE_HPP__ 1

#include <exception>
#include <functional>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

#include "formatter.hpp"
#include "learner.hpp"
#include "types.hpp"

namespace pie {

using std::accumulate;
using std::function;
using std::pair;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

template <typename res_t>
class exc_or_res {
  public:
    bool has_exc() { return exc; }
    res_t get_res() {
      if(exc)   throw std::runtime_error("Function threw exception.");
      else      return res;
    }
    bool is(const res_t & r) { return (!exc) && (res == r); }

    exc_or_res() : exc(true) {}
    exc_or_res(res_t r) : exc(false), res(r) {}

  private:
    bool exc;
    res_t res;
};

template <typename arg_t, typename res_t>
exc_or_res<res_t>
get_func_output(const function<res_t(arg_t)> & f,
                const arg_t & t) {
  try {
    return exc_or_res<res_t>(f(t));
  } catch (...) {
    return exc_or_res<res_t>();
  }
}

template <typename arg_t, typename res_t, class Formatter, class Learner>
class PIE {
  static_assert(types::is_base_of_template<Formatter, fmt::FormatterBase>::value,
                "Unrecognized Formatter!");
  static_assert(types::is_base_of_template<Learner, bfl::BFLBase>::value,
                "Unrecognized Learner!");

  public:
    using ann_t = typename Formatter::result_type;
    using feature_t = pair<function<exc_or_res<bool>(arg_t)>, ann_t>;
    using post_t = pair<function<bool(arg_t, exc_or_res<res_t>)>, ann_t>;

    ann_t infer() const {
      auto && res = learner->learn();
      vector<ann_t> features_ann;
      for(auto && f : features) features_ann.push_back(f.second);
      return Formatter(features_ann, learner->f_offset).format(res.second);
    }

    PIE & add_test(const arg_t & t) {
      (*learner) += {
        0, post.first(t, get_func_output(func, t)),
          accumulate(features.begin(), features.end(), vector<bool>(),
                     [&t](vector<bool> & fv, feature_t & f)
                       { fv.push_back(f.first(t).is(true)); return fv; })
      };
      return *this;
    }

    PIE& add_feature(pair<function<bool(arg_t)>, ann_t> && f) {
      features.push_back({[&f](arg_t a) { return get_func_output(f, a); },
                          f.second});

      Learner * new_learner = new Learner(tests.size(), features.size());
      for(auto i = 0; i < tests.size(); ++i) {
        auto && t_info = (*learner)(i);
        t_info.features.push_back(get_func_output(f, tests[i]).is(true));
        (*new_learner) += t_info;
      }

      learner.reset(new_learner);
      return *this;
    }

    PIE(vector<arg_t> && ts, function<res_t(arg_t)> && f, const post_t & p,
        const vector<pair<function<bool(arg_t)>, ann_t>> & fs)
    : tests(ts), post(p), func(f), learner(new Learner(ts.size(), fs.size())) {
      orig_tests = ts.size();
      orig_features = fs.size();

      /* save exception-safe features */
      for(auto && fi : fs)
        features.push_back({
          [&fi](arg_t a) { return get_func_output(fi.first, a); },
          fi.second});

      for(auto && t : ts)
        (*learner) += {
          accumulate(features.begin(), features.end(), vector<bool>(),
                     [&t](vector<bool> & fv, feature_t & f)
                       { fv.push_back(f.first(t).is(true)); return fv; }),
          0, post.first(t, get_func_output(func, t))
        };
    }

  protected:
    int orig_tests, orig_features;

    vector<arg_t> tests;
    unique_ptr<Learner> learner;
    const function<res_t(arg_t)> func;
    const post_t post;
    vector<feature_t> features;
};

}

#endif

#ifndef __FEATURES_CPP__

#define __FEATURES_CPP__ 1

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace gen {

using std::function;
using std::pair;
using std::string;
using std::vector;

constexpr static auto MUTUALLY_EXCLUSIVE_ONLY = true;

namespace {

template <typename T>
class FeaturesBase {
  public:
    FeaturesBase(vector<string> names) : args(names) {}

    template <typename Formatter, typename features_t>
    features_t operator[] (Formatter formatter) const;

  protected:
    vector<string> args;
};

}

template <typename Integer>
class features : public FeaturesBase<Integer> {
  public:
    using FeaturesBase<Integer>::FeaturesBase;

    template <typename Formatter, typename features_t = vector<
               pair<function<bool(Integer)>, typename Formatter::result_type>>>
    features_t operator[] (Formatter formatter) const {
      auto a = formatter.variable(this->args[0]);
      auto b = formatter.constant(0);

      features_t res = {
        {[](Integer a) { return a > 0; }, formatter.GT(a, b)},
        {[](Integer a) { return a == 0; }, formatter.EQ(a, b)},
      };

      if(!MUTUALLY_EXCLUSIVE_ONLY) {
        res.push_back({[](Integer a) { return a < 0; }, formatter.LT(a, b)});
      }

      return res;
    }
};

}

#endif

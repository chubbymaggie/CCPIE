#include <iostream>
#include <unordered_set>
#include <vector>

#include <boost/optional.hpp>

#include "pie.h"

int main() {
  using Formatter = pie::formats::Human<>;
  using Learner = pie::bfl::SimpleLearner<>;

  INFO << "Welcome to PIE!";
  pie::PIEngine<int, int, Formatter::FormatT> pie(

      pie::gen::Features<int>({"i"})[Formatter()],

      [](int i) { return i > 0 ? i : -i; },

      {[](int i, boost::optional<int> r) { return r && i == *r; }, "identity"},

      {0, 1, -1, 2, -300, 512});

  auto pre = pie.inferCNF<Learner, Formatter>();
  if (pre.first != pie::bfl::PASS)
    ERROR << "Precondition Inference Failure";
  else
    INFO << "Inferred Precondition = " << pre.second;

  INFO << "Exiting PIE. Good-bye!";
}

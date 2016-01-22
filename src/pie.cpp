#include <iostream>
#include <unordered_set>
#include <vector>

#include <boost/optional.hpp>

#include "pie.h"

int main() {
  using Formatter = pie::formats::Human<std::list>;
  using Learner = pie::bfl::SimpleLearner<>;

  INFO << "Welcome to PIE!";
  pie::PIEngine<int, int, Formatter::FormatT> pie(

      pie::gen::Features<int>({"i"})[Formatter()],

      [](int i) -> int { return i > 0 ? i : -i; },

      {[](int i, boost::optional<int> r) { return r && i == *r; }, "identity"},

      {0, 1, -1, 2, -300, 512});

  std::cout << pie.inferCNF<Learner, Formatter>() << std::endl;

  INFO << "Exiting PIE. Good-bye!";
}

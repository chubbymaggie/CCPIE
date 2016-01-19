#include <iostream>
#include <unordered_set>
#include <vector>

#include <boost/optional.hpp>

#include "pie.h"

int main() {
  using Formatter = pie::formats::Human<std::list>;
  using Learner = pie::bfl::SimpleLearner<>;

  pie::PIEngine<int, bool, Formatter::FormatT> pie(

      pie::gen::Features<int>({"i"})[Formatter()],

      [](int i) -> bool { return i % 2 == 0; },

      {[](int i, boost::optional<bool> r) { return r && !(*r); }, "not even"},

      {1, 0, -2});

  std::cout << pie.inferCNF<Learner>(Formatter()) << std::endl;

  return 0;
}

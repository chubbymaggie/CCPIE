#include <iostream>

#include "features.hpp"
#include "formatter.hpp"
#include "learner.hpp"
#include "pie.hpp"
#include "types.hpp"

using pie::exc_or_res;
using pie::PIE;

using types::clause_id_t;
using types::feature_id_t;
using types::test_id_t;

int main() {
  using Formatter = fmt::SMTLIB2Formatter;
  using Learner = bfl::PACLearner<feature_id_t, test_id_t, clause_id_t>;

  PIE<int, int, Formatter, Learner> pie(
    {1, -1, 0, -2, 2},
    [](int i) -> int { return i > 0 ? i : -i; },
    {[](int i, exc_or_res<int> r) { return r.is(i); }, "equal output"},
    gen::features<int>({"i"})[Formatter()]
  );

  std::cout << pie.infer() << std::endl;

  return 0;
}

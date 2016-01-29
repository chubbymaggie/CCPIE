#include <iostream>
#include <random>
#include <vector>

#include <boost/optional.hpp>

#include "pie.h"

int main() {
  using Formatter = pie::formats::Human<>;
  using Learner = pie::bfl::SimpleLearner<>;

  std::random_device rd;
  std::default_random_engine rand_gen(rd());
  std::uniform_int_distribution<int> udist(-5, 5);

  INFO << "Welcome to PIE Runner!";

  {
    pie::PIEngine<int, int, Formatter::FormatT> pie(

        pie::gen::Features<int>({"i"})[Formatter()],

        [](int i) { return i > 0 ? i : -i; },

        {[](int i, boost::optional<int> r) { return r && i == *r; },
         "identity"},

        pie::gen::testSequence<std::vector, int>(1024, udist, rand_gen));

    auto pre = pie.inferCNF<Learner, Formatter>();
    if (pre.first != pie::bfl::PASS)
      ERROR << "Precondition Inference Failure";
    else {
      std::cout << pre.second << std::endl;
      INFO << "Inferred Precondition = " << pre.second;
    }
  }

  INFO << "Exiting PIE. Good-bye!";
}

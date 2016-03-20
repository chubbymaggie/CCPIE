#include <iostream>
#include <random>
#include <tuple>
#include <vector>

#include <boost/optional.hpp>

#include "pie.h"

int main() {
  using namespace std;
  using namespace pie;

  using Formatter = formats::Human<>;
  using Learner = bfl::SimpleLearner;

  random_device rd;
  default_random_engine rand_gen(rd());
  uniform_int_distribution<int> udist(-5, 5);

  INFO << "Welcome to PIE Runner!";

  {
    PIEngine<int, Formatter, int> pie(

        [](const int & i) { return i > 0 ? i : -i; },

        {[](const int & input, const boost::optional<int> & result) {
           return result && input == *result;
         },
         "identity"},

        gen::testVector<tuple<int>>(1024, udist, rand_gen),

        gen::Features<Formatter, int>({"i"})[true]);

    auto pre = pie.inferCNF<Learner>();
    if (pre.first != bfl::PASS)
      ERROR << "Precondition Inference Failure";
    else {
      cout << pre.second << endl;
      INFO << "Inferred Precondition = " << pre.second;
    }
  }

  INFO << "Exiting PIE. Good-bye!";
}

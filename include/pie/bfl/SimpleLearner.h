#ifndef __PIE_BFL_SIMPLELEARNER_H__
#define __PIE_BFL_SIMPLELEARNER_H__

#include <exception>
#include <unordered_set>
#include <vector>

#include "pie/config.h"

#include "pie/Log.h"

#include "pie/bfl/ILearner.h"

namespace pie {
namespace bfl {

class SimpleLearner : public ILearner<SimpleLearner> {
public:
  using __base = ILearner<SimpleLearner>;

  using CNF = typename __base::CNF;
  using LearnerResult = typename __base::LearnerResult;

  class bad_function_error : public std::logic_error {
  public:
    bad_function_error() : logic_error("Impossible boolean function!") {}
  };

  SimpleLearner(const FeatureID & nfeature, bool s = false)
      : ILearner<SimpleLearner>(nfeature), strengthen(s) {
    INFO << "SimpleLearner instantiated with "
         << static_cast<int_fast64_t>(nfeature) << " F";
    DEBUG << indent(2) << "Offset for feature-ids = "
          << static_cast<int_fast64_t>(featureIndexOffset());
  }

  LearnerResult learnCNF() const;

  FeatureID featureIndexOffset() const { return 1; }

protected:
  CNF genAllClauses(LiteralID max_clause_size,
                    FeatureID max_feature_id,
                    bool negated_vars = true) const;

  std::unordered_set<ClauseID>
  pruneClausesWithPositives(std::unordered_set<ClauseID> && conj,
                            const std::unordered_set<BitVector> & pos) const;

  std::unordered_set<ClauseID>
  pruneClausesWithNegatives(std::unordered_set<ClauseID> && conj,
                            std::unordered_set<BitVector> & neg) const;

  std::unordered_set<ClauseID>
  learnStrongConjunctionOnAllClauses(std::unordered_set<ClauseID> && conj,
                                     const std::unordered_set<BitVector> & pos,
                                     std::unordered_set<BitVector> & neg) const;

  std::unordered_set<ClauseID>
  learnConjunctionOnAllClauses(CNF & clauses,
                               std::unordered_set<BitVector> & pos,
                               std::unordered_set<BitVector> & neg) const;

private:
  bool strengthen;
};

} // namespace bfl
} // namespace pie

#include "pie/bfl/SimpleLearner.hpp"

#endif

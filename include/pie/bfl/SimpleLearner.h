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

template <template <typename...> typename SeqContainer = std::vector,
          template <typename...> typename UniqueContainer = std::unordered_set>
class SimpleLearner
    : public ILearner<SimpleLearner<SeqContainer, UniqueContainer>,
                      SeqContainer> {
public:
  using __base =
      ILearner<SimpleLearner<SeqContainer, UniqueContainer>, SeqContainer>;
  using LearnerResult = typename __base::LearnerResult;

  using CNF = typename __base::CNF;
  using RndAccessCNF = std::vector<std::vector<FeatureID>>;

  class bad_function_error : public std::logic_error {
  public:
    bad_function_error() : logic_error("Impossible boolean function!") {}
  };

  using ILearner<SimpleLearner<SeqContainer, UniqueContainer>,
                 SeqContainer>::ILearner;

  SimpleLearner(FeatureID nfeature)
      : ILearner<SimpleLearner<SeqContainer, UniqueContainer>, SeqContainer>(
            nfeature) {
    INFO << "SimpleLearner instantiated with "
         << static_cast<int_fast64_t>(nfeature) << " F";
    DEBUG << indent(2) << "Offset for feature-ids = "
          << static_cast<int_fast64_t>(featureIndexOffset());
  }

  LearnerResult learnCNF() const;

  FeatureID featureIndexOffset() const { return 1; }

  static RndAccessCNF genAllClauses(LiteralID max_clause_size,
                                    FeatureID max_feature_id,
                                    bool negated_vars = true);

  static UniqueContainer<ClauseID>
  pruneClausesWithPositives(UniqueContainer<ClauseID> && conj,
                            RndAccessCNF & clauses,
                            const UniqueContainer<BitVector> & pos);

  static UniqueContainer<ClauseID>
  pruneClausesWithNegatives(UniqueContainer<ClauseID> && conj,
                            RndAccessCNF & clauses,
                            UniqueContainer<BitVector> & neg);

  static UniqueContainer<ClauseID>
  learnStrongConjunctionOnAllClauses(UniqueContainer<ClauseID> && conj,
                                     RndAccessCNF & clauses,
                                     const UniqueContainer<BitVector> & pos,
                                     UniqueContainer<BitVector> & neg);

  static UniqueContainer<ClauseID>
  learnConjunctionOnAllClauses(RndAccessCNF & clauses,
                               UniqueContainer<BitVector> & pos,
                               UniqueContainer<BitVector> & neg,
                               bool strengthen = false);
};

} // namespace bfl
} // namespace pie

#include "pie/bfl/SimpleLearner.hpp"

#endif

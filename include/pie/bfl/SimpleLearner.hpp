#ifndef __PIE_BFL_SIMPLELEARNER_HPP__
#define __PIE_BFL_SIMPLELEARNER_HPP__

#include <algorithm>
#include <utility>

#include "pie/config.h"

#include "pie/Log.h"

namespace pie {
namespace bfl {

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
typename SimpleLearner<SeqContainer, UniqueContainer>::LearnerResult
SimpleLearner<SeqContainer, UniqueContainer>::learnCNF() const {
  if (this->conflictedTests().size()) return {BAD_FUNCTION, {}};

  for (auto max_clause_size = 3; max_clause_size < CLAUSE_SIZE_UPPER_BOUND;
       ++max_clause_size) {
    auto clauses = genAllClauses(max_clause_size, this->feature_count);

    UniqueContainer<BitVector> pos_vectors, neg_vectors;
    for (auto && t : this->test_set) {
      BitVector vec;
      for (auto && clause : clauses) {
        bool sat = false;
        for (auto && f_id : clause) {
          if ((f_id > 0 && t.features[f_id - 1]) ||
              (f_id < 0 && !(t.features[-f_id - 1]))) {
            sat = true;
            break;
          }
        }
        vec.push_back(sat);
      }

      (t.result ? pos_vectors : neg_vectors).insert(vec);
    }

    try {
      CNF result;
      using ClauseT = typename CNF::value_type;

      DEBUG << "Learning conjunction on clauses:";
      auto learned_cnf =
          learnConjunctionOnAllClauses(clauses, pos_vectors, neg_vectors);

      for (auto & clause : learned_cnf) {
        // TODO: Conditionally avoid copy if SeqContainer is std::vector
        result.push_back(
            ClauseT(clauses[clause].cbegin(), clauses[clause].cend()));
      }
      DEBUG << "Learned CNF = " << static_cast<const CNF &>(result);

      return {PASS, result};
    } catch (const bad_function_error & e) {
      return {BAD_FUNCTION, {}};
    }
  }
  return {FAIL, {}};
}

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
typename SimpleLearner<SeqContainer, UniqueContainer>::RndAccessCNF
SimpleLearner<SeqContainer, UniqueContainer>::genAllClauses(
    LiteralID max_clause_size, FeatureID max_feature_id, bool negated_vars) {
  RndAccessCNF clauses;
  if (max_clause_size > max_feature_id) max_clause_size = max_feature_id;

  for (LiteralID size = 0; size <= max_clause_size; ++size) {
    BitVector select(max_feature_id);
    std::fill(select.begin(), select.end() - max_feature_id + size, true);

    do {
      RndAccessCNF::value_type clause;
      for (FeatureID i = 0; i < max_feature_id; ++i)
        if (select[i]) clause.push_back(i + 1);
      clauses.push_back(clause);

      if (negated_vars) {
        for (LiteralID subsize = 1; subsize <= size; ++subsize) {
          BitVector neg(size);
          std::fill(neg.begin(), neg.end() - size + subsize, true);
          do {
            RndAccessCNF::value_type nclause;
            for (LiteralID i = 0; i < size; ++i)
              if (neg[i])
                nclause.push_back(-clause[i]);
              else
                nclause.push_back(clause[i]);
            clauses.push_back(nclause);
          } while (std::prev_permutation(neg.begin(), neg.end()));
        }
      }
    } while (std::prev_permutation(select.begin(), select.end()));
  }

  DEBUG << "Generated " << clauses.size()
        << " clauses for K <= " << static_cast<int_fast64_t>(max_clause_size)
        << " over " << (negated_vars ? "2*" : "")
        << static_cast<int_fast64_t>(max_feature_id) << " Vars";
  {
    auto i = 0;
    for (auto c = clauses.cbegin(); c != clauses.end(); ++c, ++i)
      DEBUG << indent(2) << i << ". " << (*c);
  }

  return clauses;
}

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
UniqueContainer<ClauseID>
SimpleLearner<SeqContainer, UniqueContainer>::pruneClausesWithPositives(
    UniqueContainer<ClauseID> && conj,
    typename SimpleLearner<SeqContainer, UniqueContainer>::RndAccessCNF &
        clauses,
    const UniqueContainer<BitVector> & pos) {
  for (auto && p : pos)
    for (auto ci = conj.begin(); ci != conj.end();)
      if (!p[*ci])
        ci = conj.erase(ci);
      else
        ++ci;

  DEBUG << indent(2)
        << "C_pos = " << static_cast<const UniqueContainer<ClauseID> &>(conj);

  return conj;
}

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
UniqueContainer<ClauseID>
SimpleLearner<SeqContainer, UniqueContainer>::pruneClausesWithNegatives(
    UniqueContainer<ClauseID> && conj,
    typename SimpleLearner<SeqContainer, UniqueContainer>::RndAccessCNF &
        clauses,
    UniqueContainer<BitVector> & neg) {
  UniqueContainer<ClauseID> result;

  while (!neg.empty()) {
    std::pair<ClauseID, TestID> max_false{0, 0};
    for (auto & c : conj) {
      TestID f = 0;
      for (auto & n : neg) {
        if (!n[c]) ++f;
      }
      if (f > max_false.second ||
          (f == max_false.second && c < max_false.first))
        max_false = {c, f};
    }

    if (max_false.second == 0) throw bad_function_error();

    result.insert(max_false.first);
    conj.erase(max_false.first);

    for (auto ni = neg.begin(); ni != neg.end();)
      if (!(*ni)[max_false.first])
        ni = neg.erase(ni);
      else
        ++ni;
  }

  DEBUG << indent(2)
        << "C_neg = " << static_cast<const UniqueContainer<ClauseID> &>(result);

  return result;
}

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
UniqueContainer<ClauseID> SimpleLearner<SeqContainer, UniqueContainer>::
    learnStrongConjunctionOnAllClauses(
        UniqueContainer<ClauseID> && conj,
        typename SimpleLearner<SeqContainer, UniqueContainer>::RndAccessCNF &
            clauses,
        const UniqueContainer<BitVector> & pos,
        UniqueContainer<BitVector> & neg) {
  UniqueContainer<ClauseID> result;

  while (!neg.empty()) {
    std::pair<ClauseID, TestID> max_false{0, 0};
    for (auto && c : conj) {
      TestID f = 0;
      for (auto && n : neg) {
        if (!n[c]) ++f;
      }
      if (f > max_false.second ||
          (f == max_false.second && c < max_false.first))
        max_false = {c, f};
    }

    if (max_false.second == 0) throw bad_function_error();

    result.insert(max_false.first);
    bool all_unsat = true;
    for (auto && p : pos) {
      bool unsat = false;
      for (auto && c : result) {
        if (!p[c]) {
          unsat = true;
          break;
        }
      }
      all_unsat &= unsat;
      if (!all_unsat) break;
    }

    if (all_unsat) result.erase(max_false.first);
    conj.erase(max_false.first);

    for (auto ni = neg.begin(); ni != neg.end();)
      if (!(*ni)[max_false.first])
        ni = neg.erase(ni);
      else
        ++ni;
  }

  DEBUG << indent(2) << "C_strong = "
        << static_cast<const UniqueContainer<ClauseID> &>(result);

  return result;
}

template <template <typename...> typename SeqContainer,
          template <typename...> typename UniqueContainer>
UniqueContainer<ClauseID>
SimpleLearner<SeqContainer, UniqueContainer>::learnConjunctionOnAllClauses(
    typename SimpleLearner<SeqContainer, UniqueContainer>::RndAccessCNF &
        clauses,
    UniqueContainer<BitVector> & pos,
    UniqueContainer<BitVector> & neg,
    bool strengthen) {
  UniqueContainer<ClauseID> conj(clauses.size());
  for (ClauseID i = 0; i < clauses.size(); ++i) conj.insert(i);

  DEBUG << indent(2)
        << "C_all = " << static_cast<const UniqueContainer<ClauseID> &>(conj);

  return (strengthen
              ? learnStrongConjunctionOnAllClauses(
                    std::move(conj), clauses, pos, neg)
              : pruneClausesWithNegatives(
                    pruneClausesWithPositives(std::move(conj), clauses, pos),
                    clauses,
                    neg));
}

} // namespace bfl
} // namespace pie

#endif

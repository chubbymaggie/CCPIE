#ifndef __LEARNER_HPP__

#define __LEARNER_HPP__ 1

#include <exception>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "types.hpp"

namespace bfl {

using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using boost::multi_index::member;

enum learner_result_t { PASS, FAIL, BAD_FUNCTION };

template <typename learner_t, typename bool_func_t,
          typename feature_id_t, typename test_id_t>
class BFLBase {
  public:
    struct test_info {
      vector<bool> features;
      test_id_t id;
      bool result;
    };

    virtual ~BFLBase() {};
    BFLBase(test_id_t ntest, feature_id_t nfeature) : feature_count(nfeature) {
      // test_vectors.reserve(ntest);
    }

    virtual const vector<bool> & operator[] (test_id_t & test_id) const {
      const auto & tv_id_view = boost::multi_index::get<by_id>(test_vectors);
      const auto & t_it = tv_id_view.find(test_id);

      if(t_it == tv_id_view.end())
            throw std::runtime_error("Invalid test_id!");
      else return t_it -> features;
    }

    virtual test_info && operator() (test_id_t & test_id) const {
      const auto & tv_id_view = boost::multi_index::get<by_id>(test_vectors);
      const auto & t_it = tv_id_view.find(test_id);

      if(t_it == tv_id_view.end())
            throw std::runtime_error("Invalid test_id!");
      else  { auto t = *t_it; return std::move(t); }
    }

    virtual learner_t & operator+= (test_info && new_test) {
      if(new_test.id == 0)   new_test.id = test_vectors.size();
      test_vectors.insert(new_test);
      return static_cast<learner_t &>(*this);
    }

    /* TODO: Implement conflict detection */
    virtual vector<unordered_set<test_id_t>> conflicted_tests() const {
      const auto & fv_view = boost::multi_index::get<by_fvector>(test_vectors);
      return {};
    }

    virtual pair<learner_result_t, bool_func_t> learn() const = 0;

    constexpr static int f_offset = 0;

  protected:
    /* Tags for accessing test_info_container indices */
    struct by_fvector   {};
    struct by_result    {};
    struct by_id        {};

    typedef boost::multi_index::multi_index_container <
      test_info,
      boost::multi_index::indexed_by <
        boost::multi_index::hashed_unique <
          boost::multi_index::tag<by_id>,
          member<test_info, test_id_t, &test_info::id>
        >,
        boost::multi_index::hashed_non_unique <
          boost::multi_index::tag<by_result>,
          member<test_info, bool, &test_info::result>
        >,
        boost::multi_index::hashed_non_unique <
          boost::multi_index::tag<by_fvector>,
          member<test_info, vector<bool>, &test_info::features>
        >
      >
    > test_info_container;

    test_info_container test_vectors;

    const feature_id_t feature_count;
};

class bad_function_error : public std::logic_error {
  public:
    bad_function_error() : logic_error("Impossible boolean function!") {}
};

template <typename feature_id_t, typename test_id_t, typename clause_id_t,
          typename clause_t = unordered_set<feature_id_t>,
          typename bool_func_t = vector<clause_t>>
class PACLearner
: public BFLBase <PACLearner<feature_id_t, test_id_t, clause_id_t>,
                  bool_func_t, feature_id_t, test_id_t> {

  using BFLBase <PACLearner<feature_id_t, test_id_t, clause_id_t>,
                 bool_func_t, feature_id_t, test_id_t>::BFLBase;

  public:
    /* TODO: Optimize? */
    static bool_func_t
    generate_clauses(unsigned max_clause_size,
                     const feature_id_t & max_feature_id,
                     bool negated_vars = true) {
      bool_func_t clauses;
      if(max_clause_size > max_feature_id) max_clause_size = max_feature_id;

      for(auto size = 0; size <= max_clause_size; ++size) {
        vector<bool> select(max_feature_id);
        std::fill(select.begin(), select.end() - max_feature_id + size, true);

        do {
          clause_t clause;
          for(auto i = 0; i < max_feature_id; ++i)
            if(select[i]) clause.insert(i+1);
          clauses.push_back(clause);

          if(negated_vars) {
            vector<feature_id_t> clause;
            for(auto i = 0; i < max_feature_id; ++i)
              if(select[i]) clause.push_back(i+1);

            for(auto subsize = 1; subsize <= size; ++subsize) {
              vector<bool> neg(size);
              std::fill(neg.begin(), neg.end() - size + subsize, true);
              do {
                clause_t nclause;
                for(auto i = 0; i < size; ++i)
                  if(neg[i]) nclause.insert(-clause[i]);
                  else       nclause.insert(clause[i]);
                clauses.push_back(nclause);
              } while(std::prev_permutation(neg.begin(), neg.end()));
            }
          }
        } while(std::prev_permutation(select.begin(), select.end()));
      }

      return clauses;
    }

    static unordered_set<clause_id_t>
    prune_with_pos(unordered_set<clause_id_t> && conj,
                   const bool_func_t & clauses,
                   const unordered_set<vector<bool>> & pos) {
      for(auto && p : pos)
        for(auto ci = conj.begin(); ci != conj.end(); )
          if(!p[*ci]) ci = conj.erase(ci); else ++ci;

      return conj;
    }

    static unordered_set<clause_id_t>
    prune_with_neg(unordered_set<clause_id_t> && conj,
                   const bool_func_t & clauses,
                   unordered_set<vector<bool>> & neg) {
      unordered_set<clause_id_t> result;

      while(!neg.empty()) {
        pair<clause_id_t, test_id_t> max_false({0, 0});
        for(auto & c : conj) {
          test_id_t f = 0;
          for(auto & n : neg) { if(!n[c]) ++f; }
          if(f > max_false.second) max_false = {c, f};
        }

        if(max_false.second == 0) throw bad_function_error();

        result.insert(max_false.first);
        conj.erase(max_false.first);

        for(auto ni = neg.begin(); ni != neg.end(); )
          if(!(*ni)[max_false.first]) ni = neg.erase(ni); else ++ni;
      }

      return result;
    }

    static unordered_set<clause_id_t>
    learn_strong_conjunction(unordered_set<clause_id_t> && conj,
                             const bool_func_t & clauses,
                             const unordered_set<vector<bool>> & pos,
                             unordered_set<vector<bool>> & neg) {
      unordered_set<clause_id_t> result;

      while(!neg.empty()) {
        pair<clause_id_t, test_id_t> max_false({0, 0});
        for(auto & c : conj) {
          test_id_t f = 0;
          for(auto & n : neg) { if(!n[c]) ++f; }
          if(f > max_false.second) max_false = {c, f};
      }

      if(max_false.second == 0) throw bad_function_error();

        bool all_unsat = true;
        for(auto && p : pos) {
          bool unsat = false;
          for(auto && c : result) { if(!p[c]) { unsat = true; break; } }
          all_unsat &= unsat;
          if(!all_unsat) break;
        }

        if(!all_unsat)  result.insert(max_false.first);
        conj.erase(max_false.first);

        for(auto ni = neg.begin(); ni != neg.end(); )
          if(!(*ni)[max_false.first]) ni = neg.erase(ni); else ++ni;
      }

      return result;
    }

    static unordered_set<clause_id_t>
    learn_conjunction(const bool_func_t & clauses,
                      unordered_set<vector<bool>> & pos,
                      unordered_set<vector<bool>> & neg,
                      bool strengthen = false) {
      unordered_set<clause_id_t> conj;

      for(clause_id_t i = 0; i < clauses.size(); ++i)
        conj.insert(i);

      return (strengthen
        ? learn_strong_conjunction(std::move(conj), clauses, pos, neg)
        : prune_with_neg(prune_with_pos(std::move(conj), clauses, pos),
                         clauses, neg));
    }

    pair<learner_result_t, bool_func_t>
    learn() const {
      if(this->conflicted_tests().size())
        return {BAD_FUNCTION, {}};

      for(auto max_clause_size = 3; max_clause_size < types::CLAUSE_SIZE_LIMIT;
          ++max_clause_size) {
        auto clauses = generate_clauses(max_clause_size, this->feature_count);

        unordered_set<vector<bool>> pos_vectors, neg_vectors;
        for(auto && t : this->test_vectors) {
          vector<bool> vec;
          for(auto && clause : clauses) {
            bool sat = false;
            for(auto && f_id : clause) {
              if((f_id > 0 && t.features[f_id-1]) ||
                 (f_id < 0 && !(t.features[-f_id-1]))) {
                   sat = true;
                   break;
              }
            }
            vec.push_back(sat);
          }

          (t.result ? pos_vectors : neg_vectors).insert(vec);
        }

        try {
          bool_func_t result;
          for(auto & clause
              : learn_conjunction(clauses, pos_vectors, neg_vectors))
            result.push_back(clauses[clause]);
          return {PASS, result};
        } catch (const bad_function_error & e) {
           return {BAD_FUNCTION, {}};
        }
      }

      return {FAIL, {}};
    }

    constexpr static int f_offset = 1;

};

}

#endif

#ifndef __PIE_BFL_ILEARNER_H__
#define __PIE_BFL_ILEARNER_H__

#include <utility>
#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/spirit/home/support/container.hpp>

#include "pie/config.h"

namespace pie {
namespace bfl {

struct TestInfo {
  BitVector features;
  TestID id;
  bool result;
};

namespace tag {

struct by_fvector {};
struct by_id {};

} // namespace tags

enum LearnerStatus { PASS, FAIL, BAD_FUNCTION };

template <typename Derived,
          template <typename...> typename SeqContainer = std::vector>
class ILearner {
public:
  using ConflictGroup = SeqContainer<TestID>;
  using CNF = SeqContainer<SeqContainer<FeatureID>>;
  using LearnerResult = std::pair<LearnerStatus, CNF>;

  virtual ~ILearner(){};
  ILearner(FeatureID nfeature) : feature_count(nfeature) {}

  virtual const BitVector & operator[](const TestID &) const;

  virtual TestInfo operator()(const TestID &) const;

  virtual Derived & operator+=(TestInfo &&);

  virtual Derived & operator<<=(SeqContainer<TestInfo> &&);

  virtual SeqContainer<ConflictGroup> conflictedTests() const;

  virtual LearnerResult learnCNF() const = 0;

  virtual FeatureID featureIndexOffset() const = 0;

protected:
  typedef boost::multi_index::multi_index_container<
      TestInfo,
      boost::multi_index::indexed_by<
          boost::multi_index::hashed_unique<
              boost::multi_index::tag<tag::by_id>,
              boost::multi_index::member<TestInfo, TestID, &TestInfo::id>>,
          boost::multi_index::hashed_non_unique<
              boost::multi_index::tag<tag::by_fvector>,
              boost::multi_index::
                  member<TestInfo, BitVector, &TestInfo::features>>>>
      TestInfoContainer;

  TestInfoContainer test_set;

  const FeatureID feature_count;
};

} // namespace bfl
} // namespace pie

#include "pie/bfl/ILearner.hpp"

#endif
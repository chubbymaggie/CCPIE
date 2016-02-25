#ifndef __PIE_BFL_ILEARNER_HPP__
#define __PIE_BFL_ILEARNER_HPP__

#include <exception>

#include <boost/multi_index_container.hpp>

namespace pie {
namespace bfl {

template <typename Derived, template <typename...> class BackSequence>
const BitVector & ILearner<Derived, BackSequence>::
operator[](const TestID & test_id) const {
  const auto & tv_id_view = boost::multi_index::get<tag::by_id>(test_set);
  const auto & t_it = tv_id_view.find(test_id);

  if (t_it == tv_id_view.end())
    throw std::runtime_error("Invalid test_id requested!");
  else
    return t_it->features;
}

template <typename Derived, template <typename...> class BackSequence>
TestInfo ILearner<Derived, BackSequence>::
operator()(const TestID & test_id) const {
  const auto & tv_id_view = boost::multi_index::get<tag::by_id>(test_set);
  const auto & t_it = tv_id_view.find(test_id);

  if (t_it == tv_id_view.end())
    throw std::runtime_error("Invalid test_id requested!");
  else
    return *t_it;
}

template <typename Derived, template <typename...> class BackSequence>
Derived & ILearner<Derived, BackSequence>::operator+=(TestInfo && new_test) {
  if (new_test.id == 0) new_test.id = test_set.size();
  test_set.insert(std::move(new_test));
  return static_cast<Derived &>(*this);
}

template <typename Derived, template <typename...> class BackSequence>
Derived &
ILearner<Derived, BackSequence>::add_new_test(const BitVector & features,
                                              const bool result) {
  test_set.emplace(features, test_set.size(), result);
  return static_cast<Derived &>(*this);
}

template <typename Derived, template <typename...> class BackSequence>
BackSequence<BackSequence<TestID>>
ILearner<Derived, BackSequence>::conflictedTests() const {
  BackSequence<BackSequence<TestID>> conflicted_groups;

  const auto & fv_view = boost::multi_index::get<tag::by_fvector>(test_set);
  for (auto fv_it = fv_view.begin(); fv_it != fv_view.end();) {
    const auto & fv = fv_it->features;
    auto bounds = fv_view.equal_range(fv);
    bool res = fv_it->result, conflict = false;

    for (fv_it = bounds.first; fv_it != bounds.second; ++fv_it) {
      if (fv_it->result != res) {
        conflict = true;
        break;
      }
    }
    fv_it = bounds.second;

    if (conflict) {
      BackSequence<TestID> group;
      for (auto & it = bounds.first; it != bounds.second; ++it)
        group.push_back(it->id);
      conflicted_groups.push_back(group);
    }
  }

  return conflicted_groups;
}

} // namespace bfl
} // namespace pie

#endif

#ifndef __PIE_BFL_ILEARNER_HPP__
#define __PIE_BFL_ILEARNER_HPP__

#include <exception>

#include <boost/multi_index_container.hpp>

namespace pie {
namespace bfl {

template <typename Derived, template <typename...> typename SeqContainer>
const BitVector & ILearner<Derived, SeqContainer>::
operator[](const TestID & test_id) const {
  const auto & tv_id_view = boost::multi_index::get<tag::by_id>(test_set);
  const auto & t_it = tv_id_view.find(test_id);

  if (t_it == tv_id_view.end())
    throw std::runtime_error("Invalid test_id requested!");
  else
    return t_it->features;
}

template <typename Derived, template <typename...> typename SeqContainer>
TestInfo ILearner<Derived, SeqContainer>::
operator()(const TestID & test_id) const {
  const auto & tv_id_view = boost::multi_index::get<tag::by_id>(test_set);
  const auto & t_it = tv_id_view.find(test_id);

  if (t_it == tv_id_view.end())
    throw std::runtime_error("Invalid test_id requested!");
  else
    return *t_it;
}

template <typename Derived, template <typename...> typename SeqContainer>
Derived & ILearner<Derived, SeqContainer>::operator+=(TestInfo && new_test) {
  if (new_test.id == 0) new_test.id = test_set.size();
  test_set.insert(std::move(new_test));
  return static_cast<Derived &>(*this);
}

template <typename Derived, template <typename...> typename SeqContainer>
Derived & ILearner<Derived, SeqContainer>::
operator<<=(SeqContainer<TestInfo> && new_tests) {
  while (!new_tests.empty()) {
    (*this) += std::move(new_tests.back());
    new_tests.pop_back();
  }
  return static_cast<Derived &>(*this);
}

template <typename Derived, template <typename...> typename SeqContainer>
SeqContainer<SeqContainer<TestID>>
ILearner<Derived, SeqContainer>::conflictedTests() const {
  SeqContainer<SeqContainer<TestID>> conflicted_groups;

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
      SeqContainer<TestID> group;
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
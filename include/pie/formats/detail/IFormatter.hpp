#ifndef __PIE_FORMATS_DETAIL_IFORMATTER_HPP__
#define __PIE_FORMATS_DETAIL_IFORMATTER_HPP__

namespace pie {
namespace formats {
namespace detail {

#include <string>
#include <vector>

template <typename Format, template <typename...> class PushBackContainer>
Format IFormatter<Format, PushBackContainer>::format(
    const IFormatter<Format, PushBackContainer>::FormatCNF & cnf) {
  PushBackContainer<Format> conjuncts;
  for (auto && clause : cnf) conjuncts.push_back(this->OR(clause));
  return this->AND(conjuncts);
}

} // namespace detail
} // namespace formats
} // namespace pie

#endif

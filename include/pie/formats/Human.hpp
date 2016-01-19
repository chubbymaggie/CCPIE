#ifndef __PIE_FORMATS_HUMAN_HPP__
#define __PIE_FORMATS_HUMAN_HPP__

#include <string>

namespace pie {
namespace formats {

template <template <typename...> typename PushBackContainer>
std::string
Human<PushBackContainer>::AND(const PushBackContainer<std::string> & v) {
  if (v.empty()) return "";
  if (v.size() == 1) return *(v.cbegin());

  auto a = v.cbegin();
  auto res = "(" + *a;
  for (++a; a != v.cend(); ++a) res += " AND " + *a;
  res += ")";
  return res;
}

template <template <typename...> typename PushBackContainer>
std::string
Human<PushBackContainer>::OR(const PushBackContainer<std::string> & v) {
  if (v.empty()) return "";
  if (v.size() == 1) return *(v.cbegin());

  auto a = v.cbegin();
  auto res = "(" + *a;
  for (++a; a != v.cend(); ++a) res += " OR " + *a;
  res += ")";
  return res;
}

} // namespace formats
} // namespace pie

#endif

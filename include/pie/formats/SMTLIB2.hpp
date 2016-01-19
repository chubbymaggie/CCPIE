#ifndef __PIE_FORMATS_SMTLIB2_HPP__
#define __PIE_FORMATS_SMTLIB2_HPP__

#include <string>

#include "pie/formats/SMTLIB2.h"

namespace pie {
namespace formats {

template <template <typename...> typename PushBackContainer>
std::string SMTLIB2<PushBackContainer>::AND(const PushBackContainer<std::string> & v) {
  if (v.empty()) return "";
  if (v.size() == 1) return *(v.cbegin());

  std::string res = "(and";
  for (auto a = v.cbegin(); a != v.cend(); ++a) res += " " + *a;
  res += ")";
  return res;
}

template <template <typename...> typename PushBackContainer>
std::string SMTLIB2<PushBackContainer>::OR(const PushBackContainer<std::string> & v) {
  if (v.empty()) return "";
  if (v.size() == 1) return *(v.cbegin());

  std::string res = "(or";
  for (auto a = v.cbegin(); a != v.cend(); ++a) res += " " + *a;
  res += ")";
  return res;
}

} // namespace formats
} // namespace pie

#endif

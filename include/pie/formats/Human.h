#ifndef __PIE_FORMATS_HUMAN_H__
#define __PIE_FORMATS_HUMAN_H__

#include <string>
#include <vector>

#include "pie/formats/detail/IFormatter.h"

namespace pie {
namespace formats {

template <template <typename...> class PushBackContainer = std::vector>
class Human : public detail::IFormatter<std::string, PushBackContainer> {
public:
  using Format = std::string;

  Format constant(const std::string & a) override { return a; }
  Format variable(const std::string & a) override { return a; }

  Format LT(const Format & a, const Format & b) override {
    return OP(a, "<", b);
  }
  Format GT(const Format & a, const Format & b) override {
    return OP(a, ">", b);
  }
  Format EQ(const Format & a, const Format & b) override {
    return OP(a, "==", b);
  }
  Format NE(const Format & a, const Format & b) override {
    return OP(a, "!=", b);
  }

  Format OR(const Format & a, const Format & b) override {
    return OP(a, "||", b);
  }
  Format AND(const Format & a, const Format & b) override {
    return OP(a, "&&", b);
  }
  Format NOT(const Format & a) override { return "~" + a; }

  Format AND(const PushBackContainer<Format> & v) override;

  Format OR(const PushBackContainer<Format> & v) override;

protected:
  Format OP(const Format & a, const Format & op, const Format & b) {
    return "(" + a + " " + op + " " + b + ")";
  }
};

} // namespace formats
} // namespace pie

#include "pie/formats/Human.hpp"

#endif

#ifndef __PIE_FORMATS_SMTLIB2_H__
#define __PIE_FORMATS_SMTLIB2_H__

#include <string>
#include <vector>

#include "pie/formats/detail/IFormatter.h"

namespace pie {
namespace formats {

template <template <typename...> typename PushBackContainer = std::vector>
class SMTLIB2 : public detail::IFormatter<std::string, PushBackContainer> {
public:
  using Format = std::string;

  /* TODO: Declare variables, Override format() to output valid SMTLIB2 file. */

  Format constant(const std::string & a) override { return a; }
  Format variable(const std::string & a) override { return a; }

  Format LT(const Format & a, const Format & b) override {
    return OP("<", a, b);
  }
  Format GT(const Format & a, const Format & b) override {
    return OP(">", a, b);
  }
  Format EQ(const Format & a, const Format & b) override {
    return OP("=", a, b);
  }
  Format NE(const Format & a, const Format & b) override {
    return NOT(EQ(a, b));
  }

  Format OR(const Format & a, const Format & b) override {
    return OP("or", a, b);
  }
  Format AND(const Format & a, const Format & b) override {
    return OP("and", a, b);
  }
  Format NOT(const Format & a) override { return "(not " + a + ")"; }

  Format AND(const PushBackContainer<Format> & v) override;

  Format OR(const PushBackContainer<Format> & v) override;

protected:
  Format OP(const Format & op, const Format & a, const Format & b) {
    return "(" + op + " " + a + " " + b + ")";
  }
};

} // namespace formats
} // namespace pie

#include "pie/formats/SMTLIB2.hpp"

#endif

#ifndef __PIE_FORMATS_DETAIL_IFORMATTER_H__
#define __PIE_FORMATS_DETAIL_IFORMATTER_H__

namespace pie {
namespace formats {
namespace detail {

#include <string>
#include <vector>

/* Interface for any Formatter class. It should specify a format function for
 * each Operator.
 *
 * TODO: Can we verify in compile-time that all Operators have been specified?
 *
 * TODO: Get rid of the "virtual" but still ensure that all functions are
 * defined by derived classes.
 *
 * TODO: Make the AND+OR handle any Container, not just PushBackContainer. This
 * would require templates (which would conflict with virtuals). So solve the
 * previous TODO first!!
 */

template <typename Format,
          template <typename...> class PushBackContainer = std::vector>
class IFormatter {
public:
  using FormatT = Format;
  using FormatCNF = PushBackContainer<PushBackContainer<Format>>;

  /* TODO: A more generic solution? Multi-methods? */
  virtual Format constant(const std::string &) = 0;
  virtual Format variable(const std::string &) = 0;

  virtual Format LT(const Format &, const Format &) = 0;
  virtual Format GT(const Format &, const Format &) = 0;
  virtual Format EQ(const Format &, const Format &) = 0;
  virtual Format NE(const Format &, const Format &) = 0;

  virtual Format OR(const Format &, const Format &) = 0;
  virtual Format AND(const Format &, const Format &) = 0;
  virtual Format NOT(const Format &) = 0;

  virtual Format AND(const PushBackContainer<Format> &) = 0;
  virtual Format OR(const PushBackContainer<Format> &) = 0;

  virtual Format format(const FormatCNF &);

  virtual ~IFormatter() = default;
};

} // namespace detail
} // namespace formats
} // namespace pie

#include "pie/formats/detail/IFormatter.hpp"

#endif

#ifndef __PIE_CONFIG_H__
#define __PIE_CONFIG_H__

#include <cstdint>

#include <type_traits>
#include <vector>

namespace pie {

using BitVector = std::vector<bool>;

using TestID = uint_fast16_t;
using FeatureID = int_fast8_t;

using ClauseID = uint_fast32_t;
using LiteralID = uint_fast8_t;

constexpr auto CLAUSE_SIZE_UPPER_BOUND = 8;

} // namespace pie

#endif

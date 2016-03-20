#ifndef __PIE_CONFIG_H__
#define __PIE_CONFIG_H__

#include <cstdint>

#include <array>
#include <functional>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace pie {

using BitVector = std::vector<bool>;

using TestID = uint_fast16_t;
using FeatureID = int_fast8_t;

using ClauseID = uint_fast32_t;
using LiteralID = uint_fast8_t;

constexpr auto CLAUSE_SIZE_UPPER_BOUND = 8;

/* === Dependent types ====================================================== */

template <typename ResT, typename... ArgT>
using TupledArgTFunction = std::function<ResT(const std::tuple<ArgT...> &)>;

template <typename ResT, typename... ArgT>
using SafeTupledArgTFunction =
    TupledArgTFunction<boost::optional<ResT>, ArgT...>;

/* ------ Features ---------------------------------------------------------- */

template <typename Formatter, typename... ArgT>
using FeatureType =
    std::pair<SafeTupledArgTFunction<bool, ArgT...>, typename Formatter::T>;

template <typename Formatter, typename... ArgT>
using RawFeatureType =
    std::pair<std::function<bool(const ArgT &...)>, typename Formatter::T>;

/* ------ PostConditions ---------------------------------------------------- */

template <typename ResT, typename Formatter, typename... ArgT>
using PostConditionType =
    RawFeatureType<Formatter, std::tuple<ArgT...>, boost::optional<ResT>>;

template <typename ResT, typename Formatter, typename... ArgT>
using RawPostConditionType =
    RawFeatureType<Formatter, ArgT..., boost::optional<ResT>>;

/* ------ Containers -------------------------------------------------------- */

template <typename... ArgT>
using TupledVector = std::vector<std::tuple<ArgT...>>;

template <typename... ArgT>
using NamesArrayFor = std::array<std::string, sizeof...(ArgT)>;

template <typename Formatter, typename... ArgT>
using FeatureVectorType = std::vector<FeatureType<Formatter, ArgT...>>;

template <typename Formatter, typename... ArgT>
using RawFeatureVectorType = std::vector<RawFeatureType<Formatter, ArgT...>>;

} // namespace pie

#endif

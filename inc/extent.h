/*
 *    Copyright 2023 The ChampSim Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EXTENT_H
#define EXTENT_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace champsim
{
struct dynamic_extent {
  std::size_t upper;
  std::size_t lower;

  constexpr dynamic_extent(std::size_t up, std::size_t low) : upper(up), lower(low) { assert(upper >= lower); }
};

struct sized_extent {
  std::size_t upper;
  std::size_t lower;

  constexpr sized_extent(std::size_t low, std::size_t size) : upper(low + size), lower(low) { assert(upper >= lower); }
};

template <std::size_t UP, std::size_t LOW>
struct static_extent {
  constexpr static std::size_t upper{UP};
  constexpr static std::size_t lower{LOW};
};

namespace detail
{
template <typename E>
constexpr bool extent_is_static = false;

template <std::size_t UP, std::size_t LOW>
constexpr bool extent_is_static<static_extent<UP, LOW>> = true;
} // namespace detail

template <typename LHS_EXTENT, typename RHS_EXTENT>
auto extent_union(LHS_EXTENT lhs, RHS_EXTENT rhs)
{
  if constexpr (detail::extent_is_static<std::decay_t<LHS_EXTENT>> && detail::extent_is_static<std::decay_t<RHS_EXTENT>>) {
    return static_extent<std::max(lhs.upper, rhs.upper), std::min(lhs.lower, rhs.lower)>{};
  } else {
    return dynamic_extent{std::max(lhs.upper, rhs.upper), std::min(lhs.lower, rhs.lower)};
  }
}

template <typename LHS_EXTENT, typename RHS_EXTENT>
auto relative_extent(LHS_EXTENT superextent, RHS_EXTENT subextent)
{
  if constexpr (detail::extent_is_static<std::decay_t<LHS_EXTENT>> && detail::extent_is_static<std::decay_t<RHS_EXTENT>>) {
    constexpr auto superextent_size = superextent.upper - superextent.lower;
    return static_extent<superextent.lower + std::min(subextent.upper, superextent_size), superextent.lower + std::min(subextent.lower, superextent_size)>{};
  } else {
    const auto superextent_size = superextent.upper - superextent.lower;
    return dynamic_extent{superextent.lower + std::min(subextent.upper, superextent_size), superextent.lower + std::min(subextent.lower, superextent_size)};
  }
}

template <std::size_t UP, typename EXTENT>
constexpr bool bounded_upper_v = false;

template <std::size_t UP, auto SUB_UP, auto SUB_LOW>
constexpr bool bounded_upper_v<UP, static_extent<SUB_UP, SUB_LOW>> = (SUB_UP <= UP);

template <std::size_t UP, typename EXTENT>
struct bounded_upper : std::bool_constant<bounded_upper_v<UP, EXTENT>> {
};

template <std::size_t LOW, typename EXTENT>
constexpr bool bounded_lower_v = false;

template <std::size_t LOW, auto SUB_UP, auto SUB_LOW>
constexpr bool bounded_lower_v<LOW, static_extent<SUB_UP, SUB_LOW>> = (SUB_LOW <= LOW);

template <std::size_t LOW, typename EXTENT>
struct bounded_lower : std::bool_constant<bounded_lower_v<LOW, EXTENT>> {
};
} // namespace champsim

#endif

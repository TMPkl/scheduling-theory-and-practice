#pragma once

#include "sched/instance.hpp"
#include <cstddef>
#include <cstdint>

namespace sched {

// Pseudolosowa instancja o m zadaniach. Ten sam (m, seed) daje tę samą instancję.
//   p_j      ~ U[1, 20]
//   s_ij     ~ U[0, 10] dla i != j, s_ii = 0
//   d_j      ~ U[0, sum(p) + sum(max setup w wierszu)]
instance generate_instance(std::size_t m, std::uint64_t seed);

}  // namespace sched

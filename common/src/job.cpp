#include "sched/job.hpp"

#include <algorithm>

namespace sched {

int total_processing_time(std::span<const Job> jobs) {
    return std::ranges::fold_left(jobs, 0, [](int acc, const Job& j) { return acc + j.p; });
}

}  // namespace sched

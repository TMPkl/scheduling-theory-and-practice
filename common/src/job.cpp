#include "sched/job.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace sched {

Job::Job(std::size_t id, int p, int d) : id_(id), p_(p), d_(d) {
    if (p < 0) {
        throw std::invalid_argument("Processing time must be non-negative");
    }
}

std::size_t Job::id() const { return id_; }
int Job::processing_time() const { return p_; }
int Job::due_date() const { return d_; }

int Job::tardiness(int completion) const {
    return std::max(0, completion - d_);
}

int total_processing_time(std::span<const Job> jobs) {
    return std::accumulate(jobs.begin(), jobs.end(), 0,
        [](int acc, const Job& j) { return acc + j.processing_time(); });
}

}  // namespace sched

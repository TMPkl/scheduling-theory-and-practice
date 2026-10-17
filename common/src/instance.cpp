#include "sched/instance.hpp"

#include <cstddef>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

namespace sched {

namespace {

std::size_t read_size(std::istream& in) {
    std::size_t n;
    if (!(in >> n)) {
        throw std::runtime_error("Failed to read instance size");
    }
    return n;
}

std::vector<Job> read_jobs(std::istream& in, std::size_t n) {
    std::vector<Job> jobs;
    jobs.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        int p, d;
        if (!(in >> p >> d)) {
            throw std::runtime_error("Failed to read job " + std::to_string(i));
        }
        jobs.emplace_back(i, p, d);
    }
    return jobs;
}

std::ifstream open_file(const std::filesystem::path& path) {
    std::ifstream f(path);
    if (!f) {
        throw std::runtime_error("Cannot open file: " + path.string());
    }
    return f;
}

}  // namespace

instance::instance(std::istream& in) : jobs_(), sm_(0) {
    const std::size_t n = read_size(in);
    jobs_ = read_jobs(in, n);
    sm_ = read_setup_matrix(in, n);
}

instance::instance(const std::filesystem::path& path) : jobs_(), sm_(0) {
    auto f = open_file(path);
    const std::size_t n = read_size(f);
    jobs_ = read_jobs(f, n);
    sm_ = read_setup_matrix(f, n);
}

instance::instance(std::vector<Job> jobs, setup_matrix sm)
    : jobs_(std::move(jobs)), sm_(std::move(sm)) {
    if (jobs_.size() != sm_.size()) {
        throw std::invalid_argument("Number of jobs does not match setup matrix size");
    }
}

void instance::write(std::ostream& out) const {
    const std::size_t n = size();
    out << n << '\n';
    for (const Job& j : jobs_) {
        out << j.processing_time() << ' ' << j.due_date() << '\n';
    }
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t k = 0; k < n; ++k) {
            out << sm_[i, k] << (k + 1 < n ? ' ' : '\n');
        }
    }
}


std::size_t instance::size() const { return jobs_.size(); }

// Zwraca {id, setup + processing} zadania niewykonanego, które kończy się w swoim due date
// i ma najmniejszy setup + processing. Gdy brak takiego: {-1, INT_MAX}.
std::vector<int> instance::quickest_next_job_w_setup(const int current_job_id, const int time,
                                                     const boost::dynamic_bitset<>& done) const {
    int best_id = -1;
    int best_total = std::numeric_limits<int>::max();

    for (std::size_t i = 0; i < size(); ++i) {
        if (done[i]) continue;
        const int setup = sm_[static_cast<std::size_t>(current_job_id), i];
        const int total = setup + jobs_[i].processing_time();
        if (time + total > jobs_[i].due_date()) continue;  // spóźnione
        if (total < best_total) {
            best_total = total;
            best_id = static_cast<int>(i);
        }
    }
    return {best_id, best_total};
}

const std::vector<Job>& instance::jobs() const { return jobs_; }
const Job& instance::job(std::size_t i) const { return jobs_.at(i); }
const setup_matrix& instance::setups() const { return sm_; }

}  // namespace sched

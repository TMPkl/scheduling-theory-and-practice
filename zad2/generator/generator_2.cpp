#include "generator_2.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <vector>
#include <stdexcept>


namespace zad2 {

Job::Job(std::size_t id, std::vector<int> p, int r, int d) : id_(id), p_(p), r_(r), d_(d) {
    if (p.size() != 5 ) {
        throw std::invalid_argument("Processing time vector must not be empty and size must be 5");
    }
}

const std::vector<int>& Job::all_processing_times() const { return p_; }

std::size_t Job::id() const {return id_;}
int Job::readiness_time() const {return r_;}
int Job::due_date() const {return d_;}

int Job::processing_time(int n) const {
    if (n < 0 || static_cast<std::size_t>(n) >= p_.size()) {
        throw std::out_of_range("Machine index out of range");
    }
    return p_[static_cast<std::size_t>(n)];
}
int Job::tardiness(int completion_time) const {
    return completion_time - d_;}




JobGenerator::JobGenerator(std::size_t num_jobs) : num_jobs_(num_jobs) {
}

std::vector<Job> JobGenerator::generate_jobs() const {
    std::vector<Job> jobs;
    std::mt19937 gen(std::random_device{}());
    std::bernoulli_distribution wybor(0.5);
    std::poisson_distribution<int> krotkie(6.3), dlugie(53.7);

    const std::vector<double> punkty{60, 185, 415, 540};
    const std::vector<double> wagi{0, 1, 1.35, 0};
    std::piecewise_linear_distribution<double> readiness(punkty.begin(), punkty.end(), wagi.begin());

    const std::vector<double> punkty_d{26.8, 41.8, 226, 241};
    const std::vector<double> wagi_d{0, 1, 0.9, 0};
    std::piecewise_linear_distribution<double> due(punkty_d.begin(), punkty_d.end(), wagi_d.begin());

    for (std::size_t i = 0; i < num_jobs_; ++i) {

        std::vector<int> processing_times(5);
        for (int& p : processing_times) {
            p = std::max(1, wybor(gen) ? dlugie(gen) : krotkie(gen));
        }
        int readiness_time = static_cast<int>(std::lround(readiness(gen)));
        // due date = r + losowy zapas; gwarantujemy, ze najkrotszy processing time miesci sie przed d
        int min_p = *std::min_element(processing_times.begin(), processing_times.end());
        int due_date = readiness_time + static_cast<int>(std::lround(due(gen)));
        due_date = std::max(due_date, readiness_time + min_p);
        
        
        jobs.emplace_back(i, processing_times, readiness_time, due_date);
    }
    return jobs;


}

}  // namespace zad2

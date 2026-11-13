#include "sched/generator.hpp"

#include <algorithm>
#include <random>
#include <regex>
#include <vector>

#include <cmath>

namespace sched {

instance generate_instance(std::size_t m, std::uint64_t seed) {
    std::mt19937_64 rng(seed);
    // std::uniform_int_distribution<int> p_dist(5, m*2*std::log(m));
    std::poisson_distribution<int> p_dist(m);

    double srednia = m * std::log(m);
    double rozs = 0.30;

    std::bernoulli_distribution wybor(0.5);
    std::poisson_distribution<int> krotkie(srednia * (1.0 - rozs));
    std::poisson_distribution<int> dlugie(srednia * (1.0 + rozs));

    auto s_dist = [&]() { return wybor(rng) ? dlugie(rng) : krotkie(rng); };

    std::vector<int> p(m);
    for (int& x : p) x = p_dist(rng);

    setup_matrix sm(m);
    int horizon = 0;
    for (int x : p) horizon += x;
    for (std::size_t i = 0; i < m; ++i) {
        int row_max = 0;
        for (std::size_t j = 0; j < m; ++j) {
            if (i == j) continue;
            sm[i, j] = s_dist();
            row_max = std::max(row_max, sm[i, j]);
        }
        horizon += row_max;
    }

   std::weibull_distribution<float> d_dist(static_cast<float>(0.9+static_cast<double>(m)/200.0), static_cast<float>(m*m-std::log(m)));
   std::vector<Job> jobs;
    jobs.reserve(m);
    for (std::size_t i = 0; i < m; ++i) {
        jobs.emplace_back(i, p[i], static_cast<int>(d_dist(rng)));
    }
    return instance(std::move(jobs), std::move(sm));
}

}  // namespace sched

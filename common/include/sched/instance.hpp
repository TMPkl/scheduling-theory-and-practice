#pragma once

#include "sched/job.hpp"
#include "sched/setup_matrix.hpp"
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <filesystem>
#include <istream>
#include <ostream>
#include <vector>

namespace sched {

// Format pliku:
//   n
//   p_0 d_0        (n linii: czas trwania, due date)
//   ...
//   macierz setupów n x n
class instance {
public:
    explicit instance(const std::filesystem::path& path);
    explicit instance(std::istream& in);
    instance(std::vector<Job> jobs, setup_matrix sm);

    std::size_t size() const;
    const std::vector<Job>& jobs() const;
    const Job& job(std::size_t i) const;
    const setup_matrix& setups() const;

    std::vector<int> quickest_next_job_w_setup(int current_job_id, int time,
                                               const boost::dynamic_bitset<>& done) const;  
    
    
    const Job& operator[](std::size_t i) const { return job(i); }


    // Zapis w formacie czytanym przez konstruktory
    void write(std::ostream& out) const;

private:
    std::vector<Job> jobs_;
    setup_matrix sm_;

};

}  // namespace sched

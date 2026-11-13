#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace zad2 {

class Job {
public:
    Job(std::size_t id, std::vector<int> p, int r, int d);

    std::size_t id() const;
    int processing_time(int n) const;  // p - czas trwania w zaleznos  ci od maszyny
    int due_date() const;         // d - due date 
    int readiness_time() const;   // r - czas gotowości

    const std::vector<int>& all_processing_times() const; 

    // Spóźnienie zadania zakończonego w chwili completion: C - d
    int tardiness(int completion) const;

private:
    std::size_t id_;
    std::vector<int> p_;
    int r_;
    int d_;

};

int total_processing_time(std::span<const Job> jobs);

class JobGenerator {
public:
    JobGenerator(std::size_t num_jobs);

    std::vector<Job> generate_jobs() const;

private:
    std::size_t num_jobs_;
    std::vector<Job> jobs_;
};




}  // namespace zzad2


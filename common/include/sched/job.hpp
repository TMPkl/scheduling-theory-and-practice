#pragma once

#include <cstddef>
#include <span>

namespace sched {

class Job {
public:
    Job(std::size_t id, int p, int d);

    std::size_t id() const;
    int processing_time() const;  // p - czas trwania
    int due_date() const;         // d - termin zakończenia

    // Spóźnienie zadania zakończonego w chwili completion: max(0, C - d)
    int tardiness(int completion) const;

private:
    std::size_t id_;
    int p_;
    int d_;
};

int total_processing_time(std::span<const Job> jobs);

}  // namespace sched

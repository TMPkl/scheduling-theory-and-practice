#pragma once

#include <span>

namespace sched {

// Placeholder — podmień na własną reprezentację zadania.
struct Job {
    int id;
    int p;  // czas wykonywania
};

int total_processing_time(std::span<const Job> jobs);

}  // namespace sched

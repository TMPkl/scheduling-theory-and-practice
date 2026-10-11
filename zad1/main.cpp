#include <print>
#include "sched/setup_matrix.hpp"

int main() {
    sched::setup_matrix m{3};
    std::println("rozmiar: {}", m.size());

    m[0,0] = 2;
    std::println("wartośc: {}", m[1,0]);
}
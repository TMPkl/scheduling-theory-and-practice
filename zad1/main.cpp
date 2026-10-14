#include <print>
#include <sstream>
#include <stdexcept>
#include "sched/setup_matrix.hpp"

namespace {

void test_valid() {
    std::istringstream in{"3\n 0 1 2\n3 0 4\n5 6 0\n"};
    
    std::size_t n;
    in >> n;

    auto m = sched::read_setup_matrix(in, 3);
    bool ok = m[0, 1] == 1 && m[1, 2] == 4 && m[2, 0] == 5 && m[2, 2] == 0;
    
    std::println("valid:         {}", ok ? "OK" : "FAIL");
}
}  // namespace

int main() {
    test_valid();
    
}

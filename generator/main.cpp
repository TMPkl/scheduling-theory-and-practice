#include "sched/generator.hpp"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <ranges>


int main() {
    for (std::size_t n : std::views::iota(50, 501) | std::views::stride(50)) {
            const auto inst = sched::generate_instance(n, n*n+1);
        std::ofstream out("/home/karol/Documents/studia/sem7/Szeregowanie/data/zad1/160269_" + std::to_string(n) + ".txt");

        if (!out) throw std::runtime_error(std::string("Cannot open "));
        inst.write(out);

    }
}

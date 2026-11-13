#include "sched/generator.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <stdexcept>
#include <string>

// Użycie: zad1_generator [katalog_wyjsciowy]   (domyślnie data/zad1)
int main(int argc, char* argv[]) {
    const std::filesystem::path dir = argc > 1 ? argv[1] : "data/zad1";
    std::filesystem::create_directories(dir);
    for (std::size_t n : std::views::iota(50, 501) | std::views::stride(50)) {
        const auto inst = sched::generate_instance(n, n * n + 1);
        const auto path = dir / ("160269_" + std::to_string(n) + ".txt");
        std::ofstream out(path);
        if (!out) throw std::runtime_error("Cannot open " + path.string());
        inst.write(out);
    }
}

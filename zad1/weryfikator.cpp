#include <cstddef>
#include <filesystem>
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "sched/instance.hpp"

// Użycie: weryfikator <instancja> <rozwiazanie>
// Rozwiązanie: linia 1 - liczba spóźnionych zadań, linia 2 - permutacja 1..n.
int run(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uzycie: " << argv[0] << " <wejscie> <wyjscie>\n";
        return 2;
    }

    const sched::instance inst{std::filesystem::path(argv[1])};
    const std::size_t n = inst.size();

    std::ifstream f(argv[2]);
    if (!f) {
        std::cerr << "Nie mozna otworzyc pliku: " << argv[2] << "\n";
        return 2;
    }

    long long claimed;
    if (!(f >> claimed)) {
        std::cout << "BLAD: brak liczby spoznionych zadan\n";
        return 1;
    }

    std::vector<std::size_t> order;
    std::vector<bool> seen(n, false);
    for (std::size_t i = 0; i < n; ++i) {
        long long id;
        if (!(f >> id)) {
            std::cout << "BLAD: za malo numerow zadan (" << i << " z " << n << ")\n";
            return 1;
        }
        if (id < 1 || static_cast<std::size_t>(id) > n) {
            std::cout << "BLAD: numer zadania poza zakresem: " << id << "\n";
            return 1;
        }
        const auto idx = static_cast<std::size_t>(id - 1);
        if (seen[idx]) {
            std::cout << "BLAD: zadanie " << id << " wystepuje wiecej niz raz\n";
            return 1;
        }
        seen[idx] = true;
        order.push_back(idx);
    }
    long long extra;
    if (f >> extra) {
        std::cout << "BLAD: nadmiarowe dane po liscie zadan\n";
        return 1;
    }

    // Pierwsze zadanie startuje w 0 bez setupu, kolejne po setupie [poprzednie, nastepne].
    long long time = 0;
    long long late = 0;
    for (std::size_t k = 0; k < n; ++k) {
        if (k > 0) time += inst.setups()[order[k - 1], order[k]];
        time += inst.job(order[k]).processing_time();
        if (time > inst.job(order[k]).due_date()) ++late;
    }

    if (late != claimed) {
        std::cout << "BLAD: zadeklarowano " << claimed << " spoznionych, faktycznie " << late << "\n";
        return 1;
    }
    std::cout << "OK: " << late << " spoznionych zadan, Cmax = " << time << "\n";
    return 0;
}

// Pierwszy argument niebędący opcją (--xxx / -t) to plik wejściowy.
const char* input_arg(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]).rfind("-", 0) != 0) return argv[i];
    }
    return "(brak)";
}

int main(int argc, char* argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << "\nPlik wejsciowy: " << input_arg(argc, argv)
                  << (argc >= 3 ? std::string("\nPlik rozwiazania: ") + argv[2] : std::string()) << "\n";
        return 1;
    }
}

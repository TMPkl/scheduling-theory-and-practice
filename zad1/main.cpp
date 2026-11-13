#include <print>
#include <sstream>
#include <stdexcept>
#include "sched/generator.hpp"
#include "sched/instance.hpp"
#include "sched/setup_matrix.hpp"
#include <filesystem>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <algorithm>
#include <utility>

namespace {

void test_valid() {
    std::istringstream in{"3\n 0 1 2\n3 0 4\n5 6 0\n"};
    
    std::size_t n;
    in >> n;

    auto m = sched::read_setup_matrix(in, 3);
    bool ok = m[0, 1] == 1 && m[1, 2] == 4 && m[2, 0] == 5 && m[2, 2] == 0;
    
    std::println("valid:         {}", ok ? "OK" : "FAIL");
}

// Przykład: wygeneruj instancję, zapisz do pliku, wczytaj z powrotem.
}  // namespace

#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include "sched/generator.hpp"
#include "sched/instance.hpp"
#include "sched/setup_matrix.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <exception>
#include <vector>




// Liczba następnych gałęzi (k) rozbudowywanych z każdego stanu oraz szerokość wiązki.
std::size_t K = 7;

// Częściowe rozwiązanie w drzewie przeszukiwań.
struct State {
    std::vector<int> order;            // id zadań (0-based)
    boost::dynamic_bitset<> done;
    int time = 0;
    int late = 0;
    int last = -1;
};

// Zwraca do K następników stanu s: zadania, które zdążą się w terminie
// (najmniejszy setup + processing); gdy żadne nie zdąży - najmniejszy setup + processing wśród spóźnionych.
std::vector<State> expand(const sched::instance& inst, const State& s) {
    struct Cand { int id; int total; };
    std::vector<Cand> on_time, late;
    for (std::size_t i = 0; i < inst.size(); ++i) {
        if (s.done[i]) continue;
        const int total = inst.setups()[static_cast<std::size_t>(s.last), i] + inst.job(i).processing_time();
        (s.time + total > inst.job(i).due_date() ? late : on_time).push_back({static_cast<int>(i), total});
    }
    const bool is_late = on_time.empty();
    auto& cands = is_late ? late : on_time;
    const std::size_t k = std::min(K, cands.size());
    std::partial_sort(cands.begin(), cands.begin() + static_cast<std::ptrdiff_t>(k), cands.end(),
                      [](const Cand& a, const Cand& b) { return a.total < b.total; });

    std::vector<State> out;
    out.reserve(k);
    for (std::size_t c = 0; c < k; ++c) {
        State n = s;
        n.time += cands[c].total;
        n.last = cands[c].id;
        n.done[static_cast<std::size_t>(n.last)] = 1;
        n.order.push_back(n.last);
        if (is_late) ++n.late;
        out.push_back(std::move(n));
    }
    return out;
}

int run(int argc, char* argv[]) {
    // Opcja --time / -t: mierzy tylko czas działania algorytmu (bez wczytywania i zapisu plików).
    bool measure = false;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--time" || a == "-t") measure = true;
        else args.push_back(a);
    }
    if (args.empty()) {
        std::cerr << "Uzycie: " << argv[0] << " [--time] <wejscie> [wyjscie]\n";
        return 1;
    }

    std::filesystem::path in = args[0];
    auto inst = sched::instance(in);
    K = inst.size()/K; 
    const auto t_start = std::chrono::steady_clock::now();

    std::vector<std::pair<int, int>> firsts;  // {stosunek, id}
    for (const auto& job : inst.jobs()) {
        if (job.due_date() < job.processing_time()) continue;
        int stosunek = static_cast<int>(std::pow(job.due_date(), 2) / (job.processing_time() - std::log(job.processing_time())));
        firsts.emplace_back(stosunek, static_cast<int>(job.id()));
    }
    if (firsts.empty()) {
        throw std::invalid_argument("Nie ma zadania, które można wykonać przed jego terminem");
    }
    std::sort(firsts.begin(), firsts.end());
    firsts.resize(std::min(firsts.size(), K));

    std::vector<State> level;
    for (const auto& [ratio, id] : firsts) {
        State s;
        s.done = boost::dynamic_bitset<>(inst.size());
        s.done[static_cast<std::size_t>(id)] = 1;
        s.order.push_back(id);
        s.time = inst.job(static_cast<std::size_t>(id)).processing_time();
        s.last = id;
        level.push_back(std::move(s));
    }

    for (std::size_t depth = 1; depth < inst.size(); ++depth) {
        std::vector<State> next;
        for (const State& s : level) {
            auto children = expand(inst, s);
            for (auto& c : children) next.push_back(std::move(c));
        }
        const std::size_t keep = std::min(K, next.size());
        std::partial_sort(next.begin(), next.begin() + static_cast<std::ptrdiff_t>(keep), next.end(),
                          [](const State& a, const State& b) {
                              return a.late != b.late ? a.late < b.late : a.time < b.time;
                          });
        next.resize(keep);
        level = std::move(next);
    }

    const State& best = *std::min_element(level.begin(), level.end(), [](const State& a, const State& b) {
        return a.late != b.late ? a.late < b.late : a.time < b.time;
    });
    const int sum_d = best.late;
    std::vector<int> order;
    for (int id : best.order) order.push_back(id + 1);


    const auto t_end = std::chrono::steady_clock::now();
    if (measure) {
        const double us = static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count());
        std::println(std::cerr, "{}\t{:d}\t{:d}", in.filename().string(), static_cast<int>((us) / 1000.0),sum_d);
    }

    std::ostream* out = &std::cout;
    std::ofstream file;
    {
        std::filesystem::path out_path;
        if (args.size() >= 2) {
            out_path = args[1];
        } else {
            // in_<index>_<n>.txt -> ./results/zad1/out_160269_<index>_<n>.txt
            std::string stem = in.stem().string();
            if (stem.rfind("in_", 0) == 0) stem.erase(0, 3);
            out_path = std::filesystem::path("results") / "zad1" / ("out_160269_" + stem + ".txt");
        }
        if (out_path.has_parent_path()) {
            std::filesystem::create_directories(out_path.parent_path());
        }
        file.open(out_path);
        if (!file) {
            std::cerr << "Nie można otworzyć pliku: " << out_path << "\n";
            return 1;
        }
        out = &file;
    }
    *out << sum_d << '\n';
    for (std::size_t i = 0; i < order.size(); ++i) {
        *out << order[i] << (i + 1 < order.size() ? ' ' : '\n');
    }

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
        std::cerr << "Blad: " << e.what() << "\nPlik wejsciowy: " << input_arg(argc, argv) << "\n";
        return 1;
    }
}

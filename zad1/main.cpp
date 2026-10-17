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

    int time = 0;
    int sum_d = 0;
    std::vector<int> order;

    std::filesystem::path in = args[0];
    auto inst = sched::instance(in);
    
    const auto t_start = std::chrono::steady_clock::now();

    boost::dynamic_bitset<> delayed_jobs(inst.size());
    boost::dynamic_bitset<> done_jobs(inst.size());

    // pierwsza instancja to to taka, że jej duedate/processing_time()  najmniejszy a zadanie uda sie ukonczyc przed jego duedate

    int next_job_id = -1;
    int min_first_due_to_processing = std::numeric_limits<int>::max();



    for(const auto& job : inst.jobs())
    {
        if(job.due_date() < job.processing_time())
        {
            delayed_jobs.set(job.id());   
            continue;
        }
        int stosunek = static_cast<int>(std::pow(job.due_date(), 2) / (job.processing_time()-log(job.processing_time())));
        if(stosunek < min_first_due_to_processing)
        {
            min_first_due_to_processing = stosunek;
            next_job_id = job.id();
        }

    }

    if(next_job_id == -1){
        throw std::invalid_argument("Nie ma zadania, które można wykonać przed jego terminem");
    }

    // std::print("Pierwsze zadanie: {}\n", id_first_job+1);
    // std::print("Stosunek: {}\n", min_first_due_to_processing);

    time += inst.job(next_job_id).processing_time();
    done_jobs[next_job_id] = 1;
    order.push_back(next_job_id + 1);

    // Faza 1: dopóki istnieje zadanie, które zdąży się skończyć w terminie,
    // bierzemy to o najmniejszym setup + processing.
    while (done_jobs.count() < inst.size())
    {
        auto next = inst.quickest_next_job_w_setup(next_job_id, time, done_jobs);
        if (next[0] == -1) break;
        time += next[1];
        next_job_id = next[0];
        done_jobs[next_job_id] = 1;
        order.push_back(next_job_id + 1);
    }

    // Faza 2: pozostałe zadania będą spóźnione - dokładamy je w kolejności najmniejszego setup + processing.
    while (done_jobs.count() < inst.size())
    {
        int best = -1;
        int best_total = std::numeric_limits<int>::max();
        for (std::size_t i = 0; i < inst.size(); ++i) {
            if (done_jobs[i]) continue;
            int total = inst.setups()[static_cast<std::size_t>(next_job_id), i] + inst.job(i).processing_time();
            if (total < best_total) { best_total = total; best = static_cast<int>(i); }
        }
        time += best_total;
        next_job_id = best;
        done_jobs[best] = 1;
        order.push_back(best + 1);
        if (time > inst.job(static_cast<std::size_t>(best)).due_date()) ++sum_d;
    }

    const auto t_end = std::chrono::steady_clock::now();
    if (measure) {
        const double us = static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count());
        std::println(std::cerr, "Czas algorytmu: {:.3f} ms", us / 1000.0);
    }

    std::ostream* out = &std::cout;
    std::ofstream file;
    {
        std::filesystem::path out_path;
        if (args.size() >= 2) {
            out_path = args[1];
        } else {
            // in_<index>_<n>.txt -> ./results/out_160269_<index>_<n>.txt
            std::string stem = in.stem().string();
            if (stem.rfind("in_", 0) == 0) stem.erase(0, 3);
            out_path = std::filesystem::path("results") / ("out_160269_" + stem + ".txt");
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

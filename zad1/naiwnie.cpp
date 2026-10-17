#include <exception>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include "sched/generator.hpp"
#include "sched/instance.hpp"
#include "sched/setup_matrix.hpp"
#include <filesystem>
#include <fstream>
#include <vector>



int run(int argc, char* argv[]) {
    if (argc < 2) {
    std::cerr << "Uzycie: " << argv[0] << " <wejscie>\n";
    return 1;
    }

    int time = 0;
    int sum_d = 0;
    std::vector<int> order;

    std::filesystem::path in = argv[1];
    auto inst = sched::instance(in);
    
    // auto& j = inst.job(1);
    // std::println("id={} p={} d={}", j.id(), j.processing_time(), j.due_date());
    // return 0;

    for (const auto& job : inst.jobs()) {
    std::println("id={} p={} d={}", job.id(), job.processing_time(), job.due_date());
    }

    for (const auto& job : inst.jobs()) {
        time += job.processing_time();
        
        if(job.due_date() < time)
        {
            sum_d += 1; 
        }
        order.push_back(job.id()+1);
        if(job.id() < inst.size()-1)
        time += inst.setups()[job.id(), job.id()+1];
    }


    std::cout<<time<<"/n"<<sum_d<<"/n";
    std::cout<<"Kolejnosc: ";
    
    for (const auto& id : order) {
        std::cout<<id<<" ";
    }

    return 0;
}

int main(int argc, char* argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << "\nPlik wejsciowy: " << (argc >= 2 ? argv[1] : "(brak)") << "\n";
        return 1;
    }
}

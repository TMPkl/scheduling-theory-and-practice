#pragma once

#include <cstddef>
#include <istream>
#include <vector>

namespace sched {

class setup_matrix {
public:
    explicit setup_matrix(std::size_t n) : n_(n), value_(n * n, 0) {}


    std::size_t size () const;

    int& operator[](std::size_t i, std::size_t j); 
    int operator[](std::size_t i, std::size_t j) const;
    
private:
    std::size_t n_;
    std::vector<int> value_;
};

// Deklaracja funkcji z .cpp: sam nagłówek i średnik, ciało jest w setup_matrix.cpp
setup_matrix read_setup_matrix(std::istream& in, std::size_t n);

}  // namespace sched
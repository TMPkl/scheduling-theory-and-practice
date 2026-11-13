#include "sched/setup_matrix.hpp"
#include <stdexcept>
#include <string>

namespace sched {

std::size_t setup_matrix::size() const {
    return n_;
}


int& setup_matrix::operator[](std::size_t i, std::size_t j) {
    if (i >= n_ || j >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return value_[i * n_+ j];
}

int setup_matrix::operator[](std::size_t i, std::size_t j) const {
    if (i >= n_ || j >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return value_[i * n_ + j];
}

setup_matrix read_setup_matrix(std::istream& in, std::size_t n) {
    setup_matrix matrix(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (!(in >> matrix[i, j])) {
                throw std::runtime_error("Failed to read setup matrix at (" +
                    std::to_string(i) + ", " + std::to_string(j) + ")");
            }
        }
        if (matrix[i, i] != 0) {
            throw std::invalid_argument("Diagonal element " + std::to_string(i) + " should be 0");
        }
    }
    return matrix;
}




}  // namespace sched


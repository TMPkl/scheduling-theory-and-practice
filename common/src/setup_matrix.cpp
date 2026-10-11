#include "sched/setup_matrix.hpp"

namespace sched {

std::size_t setup_matrix::size() const {
    return n_;
}


int& setup_matrix::operator[](std::size_t i, std::size_t j) {
    return value_[i * n_+ j];
}

int setup_matrix::operator[](std::size_t i, std::size_t j) const {
    return value_[i * n_ + j];
}

}  // namespace sched

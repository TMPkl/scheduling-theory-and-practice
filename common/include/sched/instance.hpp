#pragma once

#include "sched/setup_matrix.hpp"
#include <cstddef>
#include <istream>
#include <vector>

#include <fstream>
#include <stdexcept>
#include <format>
#include <filesystem>

namespace sched { 

    class instance {
    public:
        instance(const std::filesystem::path& path);

    private:
        std::size_t n_;
        setup_matrix sm_;
        /*TODO: Jobs jako zbiór tasks czy jakoś tak*/

    };
}   
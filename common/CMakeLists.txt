add_library(sched_common STATIC
    src/generator.cpp
    src/instance.cpp
    src/job.cpp
    src/setup_matrix.cpp
)

target_include_directories(sched_common PUBLIC include)
target_link_libraries(sched_common PRIVATE project_warnings)

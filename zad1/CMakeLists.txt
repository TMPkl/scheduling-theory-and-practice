add_executable(zad1 main.cpp)
target_link_libraries(zad1 PRIVATE Boost::headers sched_common project_warnings)

add_executable(naiwnie naiwnie.cpp)
target_link_libraries(naiwnie PRIVATE sched_common project_warnings)

add_executable(weryfikator weryfikator.cpp)
target_link_libraries(weryfikator PRIVATE sched_common project_warnings)

add_executable(zad1_generator generator.cpp)
target_link_libraries(zad1_generator PRIVATE sched_common project_warnings)

# cmake --build <build> --target zad1_generate_data
add_custom_target(zad1_generate_data
    COMMAND zad1_generator ${CMAKE_SOURCE_DIR}/data/zad1
    COMMENT "Generowanie instancji do data/zad1"
    VERBATIM)

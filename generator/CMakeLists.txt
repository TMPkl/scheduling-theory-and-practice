add_executable(generator
    main.cpp
)

target_link_libraries(generator PRIVATE sched_common project_warnings)

# `cmake --build <build> --target generate_data` 
set(GENERATE_SIZES 50 100 150 200 250 300 350 400 450 500)
set(GENERATE_DIR ${CMAKE_SOURCE_DIR}/data/zad1)

set(GENERATE_COMMANDS)
foreach(n IN LISTS GENERATE_SIZES)
    math(EXPR seed "${n} * 2")
    list(APPEND GENERATE_COMMANDS
        COMMAND generator ${n} ${seed} ${GENERATE_DIR}/160269_${n}.txt)
endforeach()

add_custom_target(generate_data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATE_DIR}
    ${GENERATE_COMMANDS}
    COMMENT "Generowanie instancji do ${GENERATE_DIR}"
    VERBATIM)

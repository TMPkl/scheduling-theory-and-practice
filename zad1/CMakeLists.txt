add_executable(zad1
    main.cpp
)
find_package(Boost 1.70 REQUIRED CONFIG)

target_link_libraries(zad1 PRIVATE Boost::headers sched_common project_warnings)
#TODO naiwne.cpp

add_executable(naiwnie naiwnie.cpp)
target_link_libraries(naiwnie PRIVATE sched_common project_warnings)

add_executable(weryfikator weryfikator.cpp)
target_link_libraries(weryfikator PRIVATE sched_common project_warnings)

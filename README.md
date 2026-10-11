# Theory and Practice of Task Scheduling

Solutions to the laboratory assignments for the course.

The course introduces the basics of task scheduling using single-machine, parallel-machine and dedicated-machine models. It covers selected scheduling algorithms based on techniques such as graph theory and geometry, and analyses scheduling problems from the perspective of computational complexity theory. The classes cover the whole process: modelling a practical problem, choosing or designing an algorithm, and evaluating its efficiency in a computational experiment.

## Structure

- `common/` - shared library (`sched_common`)
- `zad1/` - assignment 1

## Building

Requires CMake, Ninja and a C++ compiler.

```
cmake --preset debug
cmake --build --preset debug
```

The `debug` preset enables sanitizers; `release` is meant for timing measurements.

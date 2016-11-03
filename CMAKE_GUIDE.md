# CMake guide:

This is a short guide to the cmake scripts used by this project.

## Custom cmake functions defined by the project:

The main CMakeLists.txt file defines a set of utility functions to define executable
targets from sourcefiles:

### `boost_polycoll_executable_setup(TARGET [NO_VALGRIND] [COMMAND_LINE_ARGUMENTS ...])`:

Given a cmake executable target (those defined by the [`add_executable()` command](https://cmake.org/cmake/help/v3.0/command/add_executable.html)) named
`<TARGET>`, configures it with the boost includedirs, C++ standard flags (C++14, C++11, etc)
and also defines a cmake target named `run_<TARGET>` that compiles and runs the executable.

Function arguments:

 - **`TARGET`**: Name of the target, i.e. "foo"
 - **`NO_VALGRIND`**: Optional. By default, the `run_XXXX` targets are run with valgrind,
 this option disabled valgrind for this `run_<TARGET>` target.
 - **`COMMAND_LINE_ARGUMENTS`**: Optional. List of space separated command line arguments the target will
 be invoked with when run through `run_XXX>`. For example `COMMAND_LINE_ARGUMENTS foo bar quux`

Example:

``` cmake
add_executable(perf perf.cpp)
boost_polycoll_executable_setup(perf NO_VALGRIND COMMAND_LINE_ARGUMENTS insert_base for_each_base)
```

### `boost_polycoll_executable_setup_easy(TARGET [NO_VALGRIND] [COMMAND_LINE_ARGUMENTS ...])`

Easy version of the `boost_polycoll_executable_setup() command above, which automatically defines
a `<TARGET>` executable target with a unique source file named `<TARGET>.cpp`.

It has the same parameters of `boost_polycoll_executable_setup()` command.

Example:

``` cmake
# Equivalent to the boost_polycoll_executable_setup() example above:
boost_polycoll_executable_setup_easy(perf NO_VALGRIND COMMAND_LINE_ARGUMENTS insert_base for_each_base)
```

For more examples, see `example/CMakeLists.txt`.

## Adding tests:

Given the structure of a test (a `test_XXX.cpp` file with the test, `test_XXX_main.cpp` with an entry
point to the test,and a `test_XXX.hpp` header), the test/CMakeLists.txt script defines some utilities to automate
the definition of new tests:

### `add_boostpolycoll_test(FEATURE)`

Defines an executable target named test_`<FEATURE>` that tests the feature named
`<FEATURE>`. It also adds a `run_test_<FEATURE>` target to build and run the test. This function has the same parameters
of `boost_polycoll_executable_setup()`.

Example:

``` cmake
# Defines an executable target named "test_capacity" with sources
# test_capacity_main.cpp, test_capacity.cpp, test_capacity.hpp,
# and test_utilities.hpp. Also defines a run target named "run_test_capacity"
add_boost_polycoll_test(capacity)
```

### Adding new tests:

What the `test/CMakeLists.txt` script does is to automatically define a `test_<FEATURE>` target for
each feature name defined in the `BOOST_POLYCOLLECTION_FEATURES` list variable. It also defines a
`test_all` (with its `run_test_all` target) target to run all tests through `test_all_main.cpp`.

So defining tests for new features is as simple as adding the name of the feature to the `BOOST_POLYCOLLECTION_FEATURES`
list. For example, given the list in this state:

``` cmake
...

set(BOOST_POLYCOLLECTION_FEATURES
    foo
    bar
)

...
```

adding a test for a new feature `quux` is as simple as adding "quux" to the list:

``` cmake
...

set(BOOST_POLYCOLLECTION_FEATURES
    foo
    bar
    quux
)

...
```

The script does all the rest.

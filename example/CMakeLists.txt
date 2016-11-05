if(NOT BOOST_POLYCOLLECTION_PERF_TESTS)
    set(BOOST_POLYCOLLECTION_PERF_TESTS all)
endif()

boostpolycoll_executable_setup_easy(perf NO_VALGRIND
COMMAND_LINE_ARGS
    ${BOOST_POLYCOLLECTION_PERF_TESTS}
)

if(NOT BOOST_POLYCOLLECTION_PERF_ONLY)
    boostpolycoll_executable_setup_easy(algorithms)
    boostpolycoll_executable_setup_easy(basic_any)
    boostpolycoll_executable_setup_easy(basic_base)
    boostpolycoll_executable_setup_easy(basic_function)
    boostpolycoll_executable_setup_easy(exceptions)
    boostpolycoll_executable_setup_easy(insertion_emplacement)
    boostpolycoll_executable_setup_easy(segmented_structure)
endif()

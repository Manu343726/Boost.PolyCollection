cmake_minimum_required(VERSION 2.8)
project(Boost.PolyCollection)

option(BOOST_POLYCOLLECTION_ENABLE_VALGRIND "Run unit tests with valgrind" OFF)

if(NOT MSVC)
    set(BOOST_POLYCOLLECTION_COMPILE_OPTIONS -std=c++11 -Wall -pedantic)
endif()
if(NOT BOOST_POLYCOLLECTION_BOOST_VERSION)
    set(BOOST_POLYCOLLECTION_BOOST_VERSION 1.60.0)
endif()
if(NOT BOOST_POLYCOLLECTION_INCLUDE_DIR)
    set(BOOST_POLYCOLLECTION_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
endif()

find_package(Boost ${BOOST_POLYCOLLECTION_BOOST_VERSION} REQUIRED)

function(boostpolycoll_executable_setup TARGET)
    cmake_parse_arguments(ARGS
        "NO_VALGRIND"
        ""
        ""
        ${ARGN}
    )

    target_include_directories(${TARGET} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${BOOST_POLYCOLLECTION_INCLUDE_DIR}"
        "${Boost_INCLUDE_DIRS}"
    )

    target_compile_options(${TARGET} PRIVATE
        ${BOOST_POLYCOLLECTION_COMPILE_OPTIONS}
    )

    # Add a custom target to automatically build and
    # run the test:

    if(NOT ARGS_NO_VALGRIND AND BOOST_POLYCOLLECTION_ENABLE_VALGRIND)
        find_program(MEMORYCHECK_COMMAND valgrind REQUIRED)

        set(valgrind_command ${MEMORYCHECK_COMMAND} --leak-check=full --track-origins=yes --trace-children=yes)
    endif()

    add_custom_target(run_${TARGET}
        COMMAND ${valgrind_command} $<TARGET_FILE_DIR:${TARGET}>/$<TARGET_FILE_NAME:${TARGET}>
        DEPENDS ${TARGET} # Build target first
        COMMENT "Running ${TARGET}..."
        VERBATIM
    )
endfunction()

add_subdirectory(example)
add_subdirectory(test)

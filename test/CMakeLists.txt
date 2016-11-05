
# NOTE: Header files are added to the test
# targets so are visible from IDE projects
# (Visual Studio, QtCreator, etc)

function(add_boostpolycoll_test FEATURE)
    set(main "${CMAKE_CURRENT_SOURCE_DIR}/test_${FEATURE}_main.cpp")

    if(EXISTS "${main}")
        if(${FEATURE}_EXTRA_SOURCES)
            message(STATUS "Found extra sources for feature test '${FEATURE}'")
            set(extraSources ${${FEATURE}_EXTRA_SOURCES})
        endif()

        add_executable(test_${FEATURE}
            test_${FEATURE}_main.cpp
            test_${FEATURE}.cpp
            test_${FEATURE}.hpp
            test_utilities.hpp
            ${extraSources}
        )

        boostpolycoll_executable_setup(test_${FEATURE})
    else()
        message(STATUS "File 'test_${FEATURE}_main.cpp' not found, no test target 'test_${FEATURE}' will be defined")
    endif()
endfunction()

set(BOOST_POLYCOLLECTION_FEATURES
    algorithm
    capacity
    comparison
    construction
    emplacement
    erasure
    insertion
    iterators
    registration
)

set(algorithm_EXTRA_SOURCES
    test_algorithm1.cpp
    test_algorithm1.hpp
    test_algorithm2.cpp
    test_algorithm2.hpp
    test_algorithm3.cpp
    test_algorithm3.hpp
)

foreach(feature ${BOOST_POLYCOLLECTION_FEATURES})
    add_boostpolycoll_test(${feature})

    list(APPEND test_all_sources
        test_${feature}.cpp
        test_${feature}.hpp
        test_utilities.hpp
    )

    if(${feature}_EXTRA_SOURCES)
        list(APPEND test_all_sources ${${feature}_EXTRA_SOURCES})
    endif()
endforeach()

message(STATUS "Test all sources:")
foreach(source ${test_all_sources})
    message("    ${source}")
endforeach()

add_executable(test_all test_all_main.cpp ${test_all_sources})
boostpolycoll_executable_setup(test_all)

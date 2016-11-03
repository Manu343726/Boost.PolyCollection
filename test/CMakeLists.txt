
# NOTE: Header files are added to the test
# targets so are visible from IDE projects
# (Visual Studio, QtCreator, etc)

function(add_boostpolycoll_test FEATURE)
    set(main "${CMAKE_CURRENT_SOURCE_DIR}/test_${FEATURE}_main.cpp")

    if(EXISTS "${main}")
        add_executable(test_${FEATURE}
            test_${FEATURE}_main.cpp
            test_${FEATURE}.cpp
            test_${FEATURE}.hpp
            test_utilities.hpp
        )

        boostpolycoll_executable_setup(test_${FEATURE})
    else()
        message(STATUS "File 'test_${FEATURE}_main.cpp' not found, no test target 'test_${FEATURE}' will be defined")
    endif()
endfunction()

set(BOOST_POLYCOLLECTION_FEATURES
    algorithm
    algorithm1
    algorithm2
    algorithm3
    capacity
    comparison
    construction
    emplacement
    erasure
    insertion
    iterators
    registration
)

foreach(feature ${BOOST_POLYCOLLECTION_FEATURES})
    add_boostpolycoll_test(${feature})

    list(APPEND test_all_sources
        test_${feature}.cpp
        test_${feature}.hpp
        test_utilities.hpp
    )
endforeach()

add_executable(test_all test_all_main.cpp ${test_all_sources})
boostpolycoll_executable_setup(test_all)

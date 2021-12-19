cmake_minimum_required(VERSION 3.2)

project(time_based_keyValue_store)

set(SOURCES main.cpp time_series_store.cpp)

add_executable(app ${SOURCES})

find_package(fmt)
target_link_libraries(app PRIVATE fmt::fmt)

add_library(catch2Main STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(catch2Main PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/${LIBS}/libCatch2Main.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/catch2/"
        )

add_library(catch2 STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(catch2 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/${LIBS}/libCatch2.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/catch2/"
        )

set(TEST_SRC test_store.cpp time_series_store.cpp)

add_executable(tests ${TEST_SRC})
target_link_libraries(tests PRIVATE catch2Main catch2)
# Set property for my_target only
set_property(TARGET tests PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
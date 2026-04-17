cmake_minimum_required(VERSION 3.15)
project(StudentSystemCore CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter>
    $<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:MSVC>:/O2>>
    $<$<CONFIG:Release>:$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O2>>
)

set(ALGORITHM_SOURCES
    backend/src/CourseScheduleGreedy.cpp
    backend/src/ClubResourceGreedy.cpp
    backend/src/ScorePredictDP.cpp
    backend/src/PathPlanDP.cpp
)

set(ALGORITHM_HEADERS
    backend/src/DataStructures.h
    backend/src/CourseScheduleGreedy.h
    backend/src/ClubResourceGreedy.h
    backend/src/ScorePredictDP.h
    backend/src/PathPlanDP.h
)

add_library(algorithms STATIC
    ${ALGORITHM_SOURCES}
    ${ALGORITHM_HEADERS}
)

target_include_directories(algorithms PUBLIC backend/src)

add_executable(StudentSystemCore backend/src/main.cpp)
target_link_libraries(StudentSystemCore PRIVATE algorithms)

add_executable(DemoTestCases backend/tests/DemoTestCases.cpp)
target_link_libraries(DemoTestCases PRIVATE algorithms)

add_executable(UnitTestCases backend/tests/TestCases.cpp)
target_link_libraries(UnitTestCases PRIVATE algorithms)

add_executable(PerformanceTests backend/tests/PerformanceTests.cpp)
target_link_libraries(PerformanceTests PRIVATE algorithms)

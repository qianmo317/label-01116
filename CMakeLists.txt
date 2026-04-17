cmake_minimum_required(VERSION 3.10)
project(StudentSystemCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4 /wd4100 /wd4244 /wd4267")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter")
endif()

set(ALGORITHM_SOURCES
    backend/src/CourseScheduleGreedy.cpp
    backend/src/ClubResourceGreedy.cpp
    backend/src/ScorePredictDP.cpp
    backend/src/PathPlanDP.cpp
)

set(ALGORITHM_HEADERS
    backend/src/CourseScheduleGreedy.h
    backend/src/ClubResourceGreedy.h
    backend/src/ScorePredictDP.h
    backend/src/PathPlanDP.h
    backend/src/DataStructures.h
)

add_library(algorithms STATIC ${ALGORITHM_SOURCES} ${ALGORITHM_HEADERS})
target_include_directories(algorithms PUBLIC backend/src)

add_executable(StudentSystemCore backend/src/main.cpp)
target_link_libraries(StudentSystemCore algorithms)
target_include_directories(StudentSystemCore PRIVATE backend/src)

add_executable(DemoTestCases backend/tests/DemoTestCases.cpp)
target_link_libraries(DemoTestCases algorithms)
target_include_directories(DemoTestCases PRIVATE backend/src)

add_executable(UnitTestCases backend/tests/TestCases.cpp)
target_link_libraries(UnitTestCases algorithms)
target_include_directories(UnitTestCases PRIVATE backend/src)

add_executable(PerformanceTests backend/tests/PerformanceTests.cpp)
target_link_libraries(PerformanceTests algorithms)
target_include_directories(PerformanceTests PRIVATE backend/src)

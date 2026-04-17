set(ALGORITHM_SOURCES
    CourseScheduleGreedy.cpp
    ClubResourceGreedy.cpp
    ScorePredictDP.cpp
    PathPlanDP.cpp
)

add_library(algorithms STATIC ${ALGORITHM_SOURCES})

add_executable(StudentSystemCore main.cpp)
target_link_libraries(StudentSystemCore PRIVATE algorithms)

cmake_minimum_required(VERSION 3.23)
project(event_statistics)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/91c3669f21cf569f13bd15569b5411e6ca9a8961.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )
file( GLOB TargetSources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} main.cpp ${TargetSources})
add_executable(${PROJECT_NAME}_test test/tests.cpp ${TargetSources})
target_link_libraries(${PROJECT_NAME}_test gtest)

cmake_minimum_required(VERSION 3.10)
project(LibraryManagementSystem VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/backend/include)

# Collect source files
file(GLOB_RECURSE MODEL_SOURCES "backend/src/models/*.cpp")
file(GLOB_RECURSE SERVICE_SOURCES "backend/src/services/*.cpp")
file(GLOB_RECURSE CONTROLLER_SOURCES "backend/src/controllers/*.cpp")
file(GLOB_RECURSE API_SOURCES "backend/src/api/*.cpp")

# Main executable
add_executable(library_system
    backend/src/main.cpp
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
    ${CONTROLLER_SOURCES}
)

# Optional: Add API as a library (for future HTTP server integration)
add_library(api_layer STATIC
    ${API_SOURCES}
)

# Link API library to main executable (if needed)
# target_link_libraries(library_system api_layer)

# Compiler warnings
if(MSVC)
    target_compile_options(library_system PRIVATE /W4)
else()
    target_compile_options(library_system PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules
install(TARGETS library_system DESTINATION bin)

# Testing (optional - for future use)
enable_testing()
# add_subdirectory(tests)

message(STATUS "Build configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Source Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "  Binary Directory: ${CMAKE_BINARY_DIR}")

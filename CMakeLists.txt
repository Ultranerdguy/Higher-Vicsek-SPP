cmake_minimum_required(VERSION 3.12)

project(HDV VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

include_directories(include)
include_directories(packages)
include_directories("${PROJECT_BINARY_DIR}/include")
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(app)

# Conditionally add tests
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()


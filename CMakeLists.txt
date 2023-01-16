cmake_minimum_required(VERSION 3.14)

project(HDV VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (CTest)

include_directories(include)
include_directories("${PROJECT_BINARY_DIR}/include")
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(app)

# Set a local install location for ease of use
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS HDV DESTINATION bin)

# Conditionally add tests
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Create an installer, for those so inclined
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License")
set(CPACK_PACKAGE_VERSION_MAJOR "${HDV_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${HDV_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

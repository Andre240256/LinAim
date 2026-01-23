cmake_minimum_required(VERSION 3.10)

project(LinAim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor)
include_directories(${GLM_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE HEADERS
    "include/*.hpp"
    "vendor/*.h"
    "vendor/*.hpp"
)

add_executable(LinAim ${SOURCES} ${HEADERS})


target_link_libraries(LinAim
    glfw   
    ${GLM_LIBRARIES} 
    dl
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
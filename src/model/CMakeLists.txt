cmake_minimum_required(VERSION 3.22)
project(Model VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(networks/matrix)
add_subdirectory(networks/graph)
add_subdirectory(networks/base)
add_subdirectory(reader)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_SOURCE_DIR}/model.cc
)

target_link_libraries(${PROJECT_NAME} PUBLIC MatrixNetwork GraphNetwork BaseNetwork ReaderEmnist)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS ${BUILD_FLAGS}
)

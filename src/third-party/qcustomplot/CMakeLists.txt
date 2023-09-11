cmake_minimum_required(VERSION 3.22)
project(QCustomPlot VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS PrintSupport REQUIRED)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_SOURCE_DIR}/qcustomplot.cc
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::PrintSupport)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS ${BUILD_FLAGS}
)


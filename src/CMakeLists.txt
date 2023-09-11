cmake_minimum_required(VERSION 3.5)
project(MLP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(BUILD_FLAGS "-Wall -Werror -Wextra -pedantic -O3")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(third-party/qcustomplot)
add_subdirectory(model)
add_subdirectory(tests)

enable_testing()
add_test(ForwardFeed tests/forward_feed)
add_test(Learn tests/learn)
add_test(LoadWeights tests/load_weights)
add_test(SaveWeights tests/save_weights)
add_test(Reader tests/reader)

set(PROJECT_SOURCES
    main.cc
    controller/controller.cc
    qclass/graph_mse/graph_mse_window.cc
    qclass/letter_plot/letters_plot.cc
    qclass/main_window/main_window.cc
    qclass/settings/settings.cc
    third-party/matrix.h
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets QCustomPlot Model)

qt_finalize_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS ${BUILD_FLAGS}
)

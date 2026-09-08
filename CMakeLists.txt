cmake_minimum_required(VERSION 3.16)

project(DrishtiDx VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(DrishtiDx
    src/main.cpp
    src/MainWindow.cpp
    include/MainWindow.h
    src/QualityPage.cpp
    include/QualityPage.h

    src/AnalysisPage.cpp
    include/AnalysisPage.h

    src/ResultPage.cpp
    include/ResultPage.h

    src/ReportPage.cpp
    include/ReportPage.h

    include/AppData.h
)

target_include_directories(DrishtiDx PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DrishtiDx PRIVATE
    Qt6::Widgets
)

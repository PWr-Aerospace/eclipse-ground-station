cmake_minimum_required(VERSION 3.16)
project(EclipseGS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

set(SOURCES
    src/MainWindow.cpp
    src/DiscoveryService.cpp
    src/DiscoveryPanel.cpp
    src/VideoStream.cpp
    src/VideoWidget.cpp
    src/main.cpp
)

set(HEADERS
    include/MainWindow.h
    include/DiscoveryService.h
    include/DiscoveryPanel.h
    include/VideoStream.h
    include/VideoWidget.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network
)
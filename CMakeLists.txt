cmake_minimum_required(VERSION 3.16)
project(EclipseGS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # Enable automatic moc generation
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# List all source files explicitly
set(SOURCES
    src/DiscoveryService.cpp
    src/VideoStream.cpp
    src/VideoReceiver.cpp
    src/main.cpp
)

# List all header files explicitly
set(HEADERS
    include/DiscoveryService.h
    include/VideoStream.h
    include/VideoReceiver.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network
)
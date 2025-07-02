cmake_minimum_required(VERSION 3.16)
project(EclipseGS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.28.5
)

FetchContent_MakeAvailable(SDL2)

set(SOURCES
    src/MainWindow.cpp
    src/DiscoveryService.cpp
    src/DiscoveryPanel.cpp
    src/VideoStream.cpp
    src/VideoWidget.cpp
    src/main.cpp
    src/GamepadController.cpp
    src/LoggerWidget.cpp   
)

set(HEADERS
    include/MainWindow.h
    include/DiscoveryService.h
    include/DiscoveryPanel.h
    include/VideoStream.h
    include/VideoWidget.h
    include/GamepadController.h
    include/LoggerWidget.h 
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    WIN32_EXECUTABLE ON
)


include_directories(${sdl2_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
    ${sdl2_SOURCE_DIR}/include
)


target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network
    SDL2-static
)

# Deploy for Windows
if (WIN32)
    find_program(QT_WINDEPLOYQT windeployqt
                 HINTS "$ENV{Qt6_DIR}/../../../bin"   
                       "${Qt6Core_DIR}/../../../bin"
                 REQUIRED)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_WINDEPLOYQT}"
                $<$<CONFIG:Debug>:--debug>$<$<CONFIG:Release>:--release>
                --compiler-runtime
                "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Running windeployqt on $<CONFIG>"
        VERBATIM
    )
endif()
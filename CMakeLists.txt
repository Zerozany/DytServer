cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# 设置 vcpkg 工具链文件
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file" FORCE)
set(VCPKG_TARGET_TRIPLET x64-windows)

project(http_server VERSION 1.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QMU_LIB "${CMAKE_CURRENT_SOURCE_DIR}/3rd/qmultimedia/lib/Qmultimediad.lib")
    file(GLOB DLL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/*.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd/qmultimedia/bin/Qmultimediad.dll"
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(QMU_LIB "${CMAKE_CURRENT_SOURCE_DIR}/3rd/qmultimedia/lib/Qmultimedia.lib")
    file(GLOB DLL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/*.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd/qmultimedia/bin/Qmultimedia.dll"
    )
endif()

find_package(FFMPEG REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network HttpServer Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network HttpServer Core Widgets)

add_subdirectory(3rd/dyt_chat)
add_subdirectory(3rd/spdlog)

file(GLOB ALL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
else()
    add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
endif()

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${ALL_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/qmultimedia/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/dyt_chat/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/dyt_chat/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/spdlog/include
    ${FFMPEG_INCLUDE_DIRS}

    # ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows-static-md/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${FFMPEG_LIBRARY_DIRS}

    # ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows-static-md/lib
)

# 链接 Qt6 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::HttpServer
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    ${FFMPEG_LIBRARIES}

    # ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows-static-md/lib/**
    ${QMU_LIB}
    dyt_chat
    spdlog
)

# 控制输出
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD # 在构建完成后执行
    COMMAND ${CMAKE_COMMAND} -E copy_if_different # 使用 CMake 的 copy_if_different 命令
    ${DLL_FILES} # 源文件
    $<TARGET_FILE_DIR:${PROJECT_NAME}> # 目标目录（可执行文件所在目录）
    COMMENT "Copying DLLs to executable directory" # 打印日志信息
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

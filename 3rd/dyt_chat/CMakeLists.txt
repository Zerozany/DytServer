cmake_minimum_required(VERSION 3.24)

set(VCPKG_TARGET_TRIPLET x64-windows-static-md)
set(DYTCHAT_NAME dyt_chat CACHE INTERNAL "dyt chat sdk name")
set(QT_ASSUME_STDERR_HAS_CONSOLE 1 CACHE STRING "qt assume stderr has console")

project(dyt_chat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DYT_BUILD_TESTS "Build tests" OFF)

include_directories(include)

find_package(Qt6Core CONFIG REQUIRED)
add_library(dyt_chat SHARED
        src/core/send_struct_impl.h
        src/dyt_chat.cpp
        src/core/data_handler.cpp
        src/core/data_handler_impl.h
        src/core/receive_struct.h
        src/core/parse_recv_data.cpp
        src/core/send_struct_handler.cpp
)

target_link_libraries(dyt_chat PRIVATE Qt6::Core)

set_target_properties(${DYTCHAT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

if(DYT_BUILD_TESTS)
        enable_testing()
        file(GLOB_RECURSE TEST_FILES include/*.h test/*.cpp)
        find_package(GTest CONFIG REQUIRED)
        add_executable(dyt_chat_test ${TEST_FILES}
                test/main.cpp)
        target_link_libraries(dyt_chat_test PRIVATE dyt_chat)
        target_link_libraries(dyt_chat_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
        add_test(AllTestsInMain dyt_chat_test)
endif()

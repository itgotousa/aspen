cmake_minimum_required(VERSION 3.26)

project(aspen)

add_definitions(-D_UNICODE -DUNICODE)

file(GLOB DUI_SRC "${PROJECT_SOURCE_DIR}/dui/*.cpp" "${PROJECT_SOURCE_DIR}/dui/*.c")
file(GLOB ASPEN_SRC "*.cpp" "*.c" "*.rc")

add_executable(${PROJECT_NAME} WIN32 ${ASPEN_SRC} ${DUI_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/wtl)





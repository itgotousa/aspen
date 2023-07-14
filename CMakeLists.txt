cmake_minimum_required(VERSION 3.26)

project(Aspen)

add_executable(${PROJECT_NAME} WIN32 Aspen.cpp stdafx.cpp Aspen.rc)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/wtl ${PROJECT_SOURCE_DIR}/dui)



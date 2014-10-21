cmake_minimum_required(VERSION 2.6)
project(logintc-c)

include_directories("${PROJECT_SOURCE_DIR}/src")

find_package(CURL REQUIRED)

add_library(logintc SHARED src/logintc/error.c src/logintc/logintc.c src/logintc/response.c src/logintc/session.c)
target_link_libraries(logintc curl)

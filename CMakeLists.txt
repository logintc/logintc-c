cmake_minimum_required(VERSION 2.6)
project(logintc-c)

include_directories("${PROJECT_SOURCE_DIR}/include")

find_package(CURL REQUIRED)

add_library(logintc SHARED src/error.c src/logintc.c src/response.c src/session.c)
target_link_libraries(logintc curl)

install(DIRECTORY include/logintc DESTINATION include)
install(TARGETS logintc DESTINATION lib)

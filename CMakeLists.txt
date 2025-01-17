cmake_minimum_required(VERSION 3.15...3.31)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(WORKING_DIR "day05")
set(MAIN_FILE "main.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(
    advent-of-code
    VERSION 2024
    LANGUAGES CXX C
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(SANITIZER_FLAGS " -Wall -Wextra -Werror -fsanitize=undefined,address -fsanitize=address -g ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
else()
    message(WARNING "AddressSanitizer is only supported with Clang in this setup.")
endif()


set(
    UTILS
    utils/hash_map.h
    utils/hash_map.c
    utils/vector.h
    utils/vector.c
    utils/pair.h
    utils/graph.h
    utils/graph.c
    utils/hash_map_status_codes.h
    utils/graph_matrix_representation.h
    utils/graph_matrix_representation.c
)

source_group("utils" FILES ${UTILS})
include_directories(./utils)
add_executable(main ${WORKING_DIR}/${MAIN_FILE} ${UTILS})
target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/utils")

file(GLOB TXT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${WORKING_DIR}/input/*.txt")

add_custom_target(copy_txt_files ALL)

foreach(TXT_FILE ${TXT_FILES})
    get_filename_component(FILENAME "${TXT_FILE}" NAME)
    add_custom_command(
        TARGET copy_txt_files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${TXT_FILE}"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FILENAME}"
    )
endforeach()

add_dependencies(main copy_txt_files)
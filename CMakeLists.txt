cmake_minimum_required(VERSION 3.28.3)
project(8085Microprocessor VERSION 1.0)

# set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CXX_WARNINGS -Wno-macro-redefined -Wall -Wextra)

# List all source files
set(SOURCE_DIR src)
set(SOURCE_FILES
    ${SOURCE_DIR}/8085_main.cpp
    ${SOURCE_DIR}/ui_builder.cpp
    ${SOURCE_DIR}/view.cpp
)

add_executable(8085 ${SOURCE_FILES})
target_link_libraries(8085 nCursesFacade)
target_compile_options(8085 PRIVATE ${CXX_WARNINGS})

add_custom_command(TARGET 8085 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:8085> ${CMAKE_SOURCE_DIR}/bin/
    COMMENT "copying 8085 to bin/"
)

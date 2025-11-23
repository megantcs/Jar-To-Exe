cmake_minimum_required(VERSION 3.10)

project(JarToDll)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        src/main.c
 "src/build_item.c" "src/build_jar_to_dll.c" "src/global_settings.c"
 "src/header/logger.h" "src/logger.c" "src/convertor_logger.c" "src/header/utils.h" "src/utils.c"  "src/compile_gnu.c"  "src/header/hex_tools.h" "src/hex_tools.c" "src/build_dll_to_exe.c" "src/header/files.h" "src/files.c" "src/header/terminal_attribute.h" "src/header/program.h" "src/runtime_check.c" "src/header/timer.h" "src/timer.c" "src/header/build_sys.h" "src/build_sys.c" "src/win32.c")
 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/template"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/template"
    COMMENT "Copying template directory to output folder"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/extensions"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/extensions"
    COMMENT "Copying template directory to output folder"
)

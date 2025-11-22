cmake_minimum_required(VERSION 3.10)

project(JarToDll)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        src/main.c
 "src/build_item.c" "src/build_jar_to_dll.c" "src/global_settings.c"
 "src/header/logger.h" "src/logger.c" "src/convertor_logger.c" "src/header/utils.h" "src/utils.c")

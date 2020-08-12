cmake_minimum_required(VERSION 3.0.0)
project(matrix VERSION 0.1.0)

set(C_STANDARD C99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(matrix SHARED ErrorCode.c Matrix.c)

include_directories(/home/dabushori/ex1_demo/build/libmatrix.so)
link_directories(/home/dabushori/ex1_demo/build/libmatrix.so)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

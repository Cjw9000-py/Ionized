cmake_minimum_required(VERSION 3.22)
project(Ionized C)

set(CMAKE_C_STANDARD 99)

include_directories(include/)

set(LIB_IONC_SOURCES
    src/lexer.c
    src/list.c
    src/config.c
)

set(IONC_SOURCES
    src/ionc.c
)

# project targets
add_library(libionc SHARED ${LIB_IONC_SOURCES})
add_executable(ionc ${IONC_SOURCES} ${LIB_IONC_SOURCES})


# tests
add_executable(test_list tests/test_list.c ${LIB_IONC_SOURCES})
add_executable(test_lexer tests/test_lexer.c ${LIB_IONC_SOURCES})
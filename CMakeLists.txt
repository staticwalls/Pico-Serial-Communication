cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/path/to/pico-sdk")

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Pico-Serial-Communication C CXX ASM)

pico_sdk_init()

add_executable(shell
    shell.c
)

target_link_libraries(shell
    pico_stdlib
    pico_cyw43_arch_none
)

pico_enable_stdio_usb(shell 1)

pico_add_extra_outputs(shell)

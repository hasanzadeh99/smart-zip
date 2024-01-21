cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ver8)

target_sources(app PRIVATE

    src/main.c
    src/BMLite_sdk/bmlite_if.c
    src/BMLite_sdk/fpc_crc.c
    src/BMLite_sdk/hal_spi.c
    src/BMLite_sdk/hcp_tiny.c
    src/BMLite_sdk/platform.c
    src/BMLite_sdk/hal_timebase.c
    src/BMLite_sdk/hal_board.c
)






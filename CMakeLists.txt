cmake_minimum_required(VERSION 3.22)

set(PICO_FLASH_SIZE_BYTES "(2* 1024 * 1024)")
include(FetchContent)
FetchContent_Declare(
    pico_sdk
    GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk.git
    GIT_TAG 2.1.0
    GIT_PROGRESS ON
)
FetchContent_Declare(
    u8g2
    GIT_REPOSITORY https://github.com/olikraus/u8g2.git
    GIT_TAG 9c841fafef11f430a225aa692944264c26a2405d # 2.36.2
    GIT_PROGRESS ON
)
FetchContent_MakeAvailable(pico_sdk u8g2)

project(
    pico_ssd1306_i2c_linkage_code
    VERSION 1.0.0
    LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(APP_NAME "${PROJECT_NAME}.elf")


file(GLOB APP_FILES CONFIGURE_DEPENDS "${APP_DIR}/*.cpp" "${APP_DIR}/*.c")

add_executable("${APP_NAME}" ${APP_FILES})
target_link_libraries("${APP_NAME}" pico_stdlib u8g2 hardware_i2c)
target_include_directories("${APP_NAME}" PRIVATE "${INC_DIR}")
pico_enable_stdio_usb("${APP_NAME}" 1)
pico_enable_stdio_uart("${APP_NAME}" 0)
pico_add_extra_outputs("${APP_NAME}")
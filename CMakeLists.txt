# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(projeto_ohmimetro C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(projeto_ohmimetro
    main.c
    lib/ssd1306.c
    lib/ws2812.c
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
        PICO_PRINTF_SUPPORT_FLOAT=1 
        PICO_STDIO_ENABLE_PRINTF=1
    )

pico_set_program_name(projeto_ohmimetro "projeto_ohmimetro")
pico_set_program_version(projeto_ohmimetro "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(projeto_ohmimetro 1)
pico_enable_stdio_usb(projeto_ohmimetro 1)

# Add the standard library to the build
target_link_libraries(projeto_ohmimetro pico_stdlib hardware_gpio hardware_i2c hardware_adc hardware_irq hardware_pio)

# Add the standard include files to the build
target_include_directories(projeto_ohmimetro PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        lib/ssd1306
        lib/ws2812
)

# Adiciona o .pio ao projeto
pico_generate_pio_header(projeto_ohmimetro ${CMAKE_CURRENT_LIST_DIR}/lib/ws2812.pio)

# Add any user requested libraries
target_link_libraries(projeto_ohmimetro 
        
        )

pico_add_extra_outputs(projeto_ohmimetro)
cmake_minimum_required(VERSION 2.8)

project(stm32f4xx_base C CXX ASM)

option(FREERTOS "Build target with include FreeRTOS" OFF)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTx_FLASH.ld")

add_definitions(-DSTM32F407xx)

set(STM32F4_FLAGS "-mthumb -march=armv7e-m -mfloat-abi=softfp -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --std=gnu99 ${STM32F4_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --std=gnu++11 ${STM32F4_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS " -T ${LINKER_SCRIPT} --specs=nosys.specs")

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/application/source" SOURCE_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/HAL/stm32f4xx_hal/Src" STM32F4XX_HAL)

set(SOURCES
    "${SOURCE_LIST}"
    "${STM32F4XX_HAL}"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/CMSIS/device/source/system_stm32f4xx.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/CMSIS/device/source/startup_stm32f407xx.s"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/stm32f4xx_hal/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/CMSIS/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/CMSIS/device/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/application/include"
)

if(FREERTOS)
    add_definitions(-DFREERTOS)

    aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source" FREERTOS_SOURCES)

    set(SOURCES "${SOURCES}"
        "${FREERTOS_SOURCES}"
        "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/MemMang/heap_2.c"
    )

    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    )
endif(FREERTOS)

add_executable(${PROJECT_NAME} ${SOURCES})

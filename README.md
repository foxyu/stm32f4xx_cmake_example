# stm32f4xx_cmake_example
Пример проекта под stm32f4xx с использованием arm-none-eabi toolchain и cmake

## Сборка
```
$ git clone https://github.com/zeProdigy/stm32f4xx_cmake_example.git
$ cd stm32f4xx_cmake_example
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
$ make
```

## Для подключения к проекту FreeRTOS включить опцию:
```
cmake -DFREERTOS=ON -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
```
При зашивке в stm32f4Discovery плата должна бодро моргать светодиодами средствами ОСРВ

## Конвертация elf -> bin
```
$ arm-none-eabi-objcopy -O binary <elf_file_name> <new_bin_file_name>
```

## Зашивка через texane st-link utility
```
$ ./st-flash write <path_to_bin_file> 0x08000000
```

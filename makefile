INCLUDE=-IFreeRTOS/Source/include/
INCLUDE+=-IFreeRTOS/Source/portable/GCC/ARM_CM4/
SRCS=$(shell fd -I -e c . FreeRTOS)
include ~/TM4C123G-template/Makefile

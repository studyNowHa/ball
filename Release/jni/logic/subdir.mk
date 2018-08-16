################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../jni/logic/mainLogic.cc \
../jni/logic/playLogic.cc 

CC_DEPS += \
./jni/logic/mainLogic.d \
./jni/logic/playLogic.d 

OBJS += \
./jni/logic/mainLogic.o \
./jni/logic/playLogic.o 


# Each subdirectory must supply rules for building sources it contributes
jni/logic/%.o: ../jni/logic/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



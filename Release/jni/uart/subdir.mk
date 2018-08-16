################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jni/uart/ProtocolParser.cpp \
../jni/uart/ProtocolSender.cpp \
../jni/uart/UartContext.cpp 

OBJS += \
./jni/uart/ProtocolParser.o \
./jni/uart/ProtocolSender.o \
./jni/uart/UartContext.o 

CPP_DEPS += \
./jni/uart/ProtocolParser.d \
./jni/uart/ProtocolSender.d \
./jni/uart/UartContext.d 


# Each subdirectory must supply rules for building sources it contributes
jni/uart/%.o: ../jni/uart/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



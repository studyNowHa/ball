################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jni/tcp_transmittion/ServerSocket.cpp \
../jni/tcp_transmittion/Socket.cpp \
../jni/tcp_transmittion/data.cpp \
../jni/tcp_transmittion/trans.cpp 

OBJS += \
./jni/tcp_transmittion/ServerSocket.o \
./jni/tcp_transmittion/Socket.o \
./jni/tcp_transmittion/data.o \
./jni/tcp_transmittion/trans.o 

CPP_DEPS += \
./jni/tcp_transmittion/ServerSocket.d \
./jni/tcp_transmittion/Socket.d \
./jni/tcp_transmittion/data.d \
./jni/tcp_transmittion/trans.d 


# Each subdirectory must supply rules for building sources it contributes
jni/tcp_transmittion/%.o: ../jni/tcp_transmittion/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



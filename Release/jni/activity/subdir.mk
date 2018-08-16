################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jni/activity/mainActivity.cpp \
../jni/activity/playActivity.cpp 

OBJS += \
./jni/activity/mainActivity.o \
./jni/activity/playActivity.o 

CPP_DEPS += \
./jni/activity/mainActivity.d \
./jni/activity/playActivity.d 


# Each subdirectory must supply rules for building sources it contributes
jni/activity/%.o: ../jni/activity/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



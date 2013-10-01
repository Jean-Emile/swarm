################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/Observateur.cpp \
../src/utils/Vector3.cpp 

OBJS += \
./src/utils/Observateur.o \
./src/utils/Vector3.o 

CPP_DEPS += \
./src/utils/Observateur.d \
./src/utils/Vector3.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_GNU_SOURCE -O0 -g3 -c -fmessage-length=0  -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Drone.cpp \
../src/Geopoint.cpp \
../src/Gui.cpp \
../src/Mavlink.cpp \
../src/Repulsive.cpp \
../src/Scheduler.cpp \
../src/Swarm.cpp \
../src/main.cpp 

OBJS += \
./src/Drone.o \
./src/Geopoint.o \
./src/Gui.o \
./src/Mavlink.o \
./src/Repulsive.o \
./src/Scheduler.o \
./src/Swarm.o \
./src/main.o 

CPP_DEPS += \
./src/Drone.d \
./src/Geopoint.d \
./src/Gui.d \
./src/Mavlink.d \
./src/Repulsive.d \
./src/Scheduler.d \
./src/Swarm.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_GNU_SOURCE -O0 -g3 -c -fmessage-length=0  -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



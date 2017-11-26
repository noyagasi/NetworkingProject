################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LockedDispatcher.cpp \
../src/OpenedDispatcher.cpp \
../src/Server.cpp \
../src/ServerManager.cpp \
../src/User.cpp 

OBJS += \
./src/LockedDispatcher.o \
./src/OpenedDispatcher.o \
./src/Server.o \
./src/ServerManager.o \
./src/User.o 

CPP_DEPS += \
./src/LockedDispatcher.d \
./src/OpenedDispatcher.d \
./src/Server.d \
./src/ServerManager.d \
./src/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/project49/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



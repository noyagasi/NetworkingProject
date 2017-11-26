################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MainTester.cpp \
../src/OverAllTest.cpp 

OBJS += \
./src/MainTester.o \
./src/OverAllTest.o 

CPP_DEPS += \
./src/MainTester.d \
./src/OverAllTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/project49/Client/src" -I"/home/user/Desktop/project49/Server/src" -I"/home/user/Desktop/project49/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RuchusLogMonitor.cpp \
../src/RuchusMonitor.cpp \
../src/SmartacFileChannel.cpp \
../src/SmartacFileChannelFactory.cpp \
../src/SmartacFormatterFactory.cpp \
../src/SmartacPatternFormatter.cpp 

OBJS += \
./src/RuchusLogMonitor.o \
./src/RuchusMonitor.o \
./src/SmartacFileChannel.o \
./src/SmartacFileChannelFactory.o \
./src/SmartacFormatterFactory.o \
./src/SmartacPatternFormatter.o 

CPP_DEPS += \
./src/RuchusLogMonitor.d \
./src/RuchusMonitor.d \
./src/SmartacFileChannel.d \
./src/SmartacFileChannelFactory.d \
./src/SmartacFormatterFactory.d \
./src/SmartacPatternFormatter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/boost/include -I/root/poco/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



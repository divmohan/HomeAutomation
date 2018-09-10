################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Libraries/CommChannel/ZMQ/src/MessageTransfer.cpp 

O_SRCS += \
../Libraries/CommChannel/ZMQ/src/MsgTransfer.o 

OBJS += \
./Libraries/CommChannel/ZMQ/src/MessageTransfer.o 

CPP_DEPS += \
./Libraries/CommChannel/ZMQ/src/MessageTransfer.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CommChannel/ZMQ/src/%.o: ../Libraries/CommChannel/ZMQ/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



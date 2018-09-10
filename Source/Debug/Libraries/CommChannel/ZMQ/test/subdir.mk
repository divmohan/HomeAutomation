################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Libraries/CommChannel/ZMQ/test/TestPubApp.cc \
../Libraries/CommChannel/ZMQ/test/TestSubApp.cc 

CC_DEPS += \
./Libraries/CommChannel/ZMQ/test/TestPubApp.d \
./Libraries/CommChannel/ZMQ/test/TestSubApp.d 

OBJS += \
./Libraries/CommChannel/ZMQ/test/TestPubApp.o \
./Libraries/CommChannel/ZMQ/test/TestSubApp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CommChannel/ZMQ/test/%.o: ../Libraries/CommChannel/ZMQ/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



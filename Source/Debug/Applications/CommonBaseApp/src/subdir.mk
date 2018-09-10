################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Applications/CommonBaseApp/src/BaseApp.cpp 

OBJS += \
./Applications/CommonBaseApp/src/BaseApp.o 

CPP_DEPS += \
./Applications/CommonBaseApp/src/BaseApp.d 


# Each subdirectory must supply rules for building sources it contributes
Applications/CommonBaseApp/src/%.o: ../Applications/CommonBaseApp/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



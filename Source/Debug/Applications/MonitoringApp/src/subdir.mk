################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Applications/MonitoringApp/src/MonitoringApp.cpp 

OBJS += \
./Applications/MonitoringApp/src/MonitoringApp.o 

CPP_DEPS += \
./Applications/MonitoringApp/src/MonitoringApp.d 


# Each subdirectory must supply rules for building sources it contributes
Applications/MonitoringApp/src/%.o: ../Applications/MonitoringApp/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



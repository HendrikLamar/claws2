


# Add inputs and outputs from these tool invocations to the build variables
# CALI_CPP_SRCS += ../src/Event.cpp
# CALI_CPP_SRCS += ../src/GlobalSettings.cxx


PICO_OBJS += ./build/src/pico.o
PICO_OBJS += ./build/src/claws.o
PICO_OBJS += ./build/src/utility.o
PICO_OBJS += ./build/src/readini.o
PICO_OBJS += ./build/src/statemachine.o
PICO_OBJS += ./build/src/database.o
PICO_OBJS += ./build/src/scpi.o
PICO_OBJS += ./build/src/n6700.o
#PICO_OBJS += ./build/src/clawsException.o
PICO_OBJS += ./build/src/clawsRun.o



#CALI_OBJS += ./build/src/channel.o
#CALI_OBJS += ./build/src/event.o
#CALI_OBJS += ./build/src/gain.o
#CALI_OBJS += ./build/src/pedestal.o
#CALI_OBJS += ./build/src/run.o
#CALI_OBJS += ./build/src/globalsettings.o
#
#ANA_OBJS += ./build/src/globalsettings.o

# CALI_CPP_DEPS += ./build/src/EVENT.d
# CALI_CPP_DEPS += ./build/src/Gloablsettings.d

# Each subdirectory must supply rules for building sources it contributes

./build/src/%.o: ./src/%.cxx
	@echo 'Building target: $@ from: $<'
	@mkdir -p './build/src'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o "$@" "$<"
	@echo 'Finished building: $@ from: $<'
	@echo ' '



	#$(CC) $(CFLAGS) $(INCLUDES)  -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

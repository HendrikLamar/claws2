-include ./src/subdir.mk

ifeq ($(shell uname -s),Linux)
	
	# pico
	CFLAGS += -I/opt/picoscope/include/ 
	LFLAGS += -L/opt/picoscope/lib -lps6000
	
	# general cpp flags
	CFLAGS += -c -fmessage-length=0 -std=c++11
	
	# ROOT
	CFLAGS += `root-config --cflags`
	LFLAGS += `root-config --libs --glibs`

	# DAQ stuff
	CFLAGS += -I./include
	LFLAGS += -L./libs

	CC = g++

endif

###################################
# run debug compilation as default

# debug: CFLAGS += -g -O0 -Wall
# debug: clean build
#
###################################
all: CFLAGS += -g -O0 -Wall
all: clean build

# run release compilation
release: CFLAGS += -O3
release: clean build


VERSION = 1.0
INCLUDES += -I"./include"
PICO_OBJS += ./build/src/main.o


# Link
build: $(PICO_OBJS)
	@echo 'Linking target: $@ from: $<'
	@mkdir -p './build/src'
	@echo 'Calling GCC C++ Linker'
	$(CC) -o "build/pico" $(PICO_OBJS) $(MAIN_OBJS) $(LFLAGS)
	@echo 'Finished linking: $@ from: $<'
	@echo ' '

# compile but don't link
build/src/%.o: ./main.cpp
	@echo 'Building target: $@ from: $<'
	@mkdir -p './build/src'
	@echo 'Calling GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -o "$@" "$<"
	@echo 'Finished building: $@ from: $<'
	@echo ' '


clean:
	@echo 'Cleaning up...'
	rm -rf './build'
	@echo ' '

doc:
	@echo 'Building docs...'
	doxygen config_doxygen
	@echo ' '

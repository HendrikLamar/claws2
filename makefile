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
	LFLAGS += 

	# boost libs
	CFLAGS +=
#	LFLAGS += -L/usr/lib/x86_64-linux-gnu/libboost_system.so.1.58.0 -L/usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.58.0
	LFLAGS += -lboost_system -lboost_filesystem

	CC = g++

endif

###################################
# run debug compilation as default

# debug: CFLAGS += -g -O0 -Wall
# debug: clean build
#
###################################
all: CFLAGS += -g -O0 -Wall
#all: build
all: clean build

# run release compilation
release: CFLAGS += -O3
release: clean build


VERSION = 1.1
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
	rsync -arz --delete ./docs -e ssh hwindel@pcbelle20.mpp.mpg.de:~/public_html/claws2_doc/
	@echo ' '

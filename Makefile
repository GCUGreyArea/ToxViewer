NAME = tox_client 

TARGET  = $(NAME)
PWD = $(shell pwd)

BUILD	   = $(PWD)/build
TESTDIR    = $(PWD)/test
BENCHDIR   = $(PWD)/benchmark
LIBDIR     = $(PWD)/lib
DOXYDIR	   = $(PWD)/docs
BUILD      = $(PWD)/build

TESTTARGET = $(PWD)/$(TESTDIR)/$(BUILD)/test_$(TARGET)
BNCTARGET  = $(PWD)/$(BENCHDIR)/$(BUILD)/benchmark_$(TARGET)
LIBTARGET  = $(PWD)/$(LIBDIR)/$(BUILD)/lib$(TARGET).so
DOCTARGET  = $(PWD)/$(DOXYDIR)/generated

SRC = $(shell find src -name '*.cpp')
OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(SRC))

INC = -I$(LIBDIR) -I$(LIBDIR)/src
LNK_PATH = $(PWD)/lib/build
    
LIBRARIES = 
CFLAGS   = -std=c11 -Wall -g $(INC)
CXXFLAGS = -std=c++17 -fPIC -Wall -g $(INC)

export NAME
export CXXFLAGS
export CFLAGS
export LNK_PATH
export INC
export LIBRARIES
export LIBTARGET

export TESTDIR
export LIBDIR
ARGS =

PWD :=$(shell pwd)
UNAME := $(shell uname)

all: $(LIBTARGET) $(TARGET)

$(TARGET) : build $(LIBTARGET) $(TESTTARGET) $(BENCHTARGET) $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BUILD)/$(TARGET) -L$(LNK_PATH) -l$(TARGET) $(LIBRARIES) -Wl,-rpath,$(LNK_PATH)

build :
	mkdir -p "$(BUILD)/src"

$(LIBTARGET) :
	cd $(LIBDIR) && make 

$(TESTTARGET) : $(LIBTARGET)
	cd $(TESTDIR) && make

$(BNCTARGET) : $(LIB)
	cd $(BENCHDIR) && make

$(DOCTARGET) :
	cd $(DOXYDIR) && make

$(BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -c $< -o $@

clean:
	cd $(TESTDIR) && make clean
	cd $(LIBDIR) && make clean
	rm -rf $(BUILD)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

docs: $(DOCTARGET)
	cd $(DOXYDIR) && doxygen Doxyfile

test: $(TESTTARGET)
	cd $(LIBDIR) && make
	cd $(TESTDIR) && make

benchmark: $(BNCTARGET)
	cd $(LIBDIR) && make
	cd $(BENCHDIR) && make

project:
	cd $(LIBDIR) && make
	cd $(TESTDIR) && make
	cd $(BENCHDIR) && make
	make

NAME  = tox_client

SOURCEDIR = src

SRC_C = $(wildcard $(SOURCEDIR)/*.c)
SRC_CPP += $(wildcard $(SOURCEDIR)/*.cpp)

BUILD = build

OBJ = $(patsubst %.cpp,$(BUILD)/%.o,$(SRC_CPP)) 
OBJ += $(patsubst %.c,$(BUILD)/%.o,$(SRC_C))

TARGET  = lib$(NAME).so
LIBDIR  =  .


UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
CC       = cc
CXX      = c++
else
CC     = gcc
CXX    = g++
endif

CFLAGS += -fPIC
CXXFLAGS += -g -fPIC

all: build $(TARGET)

build:
	mkdir -p build/src

$(TARGET) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -shared -o $(BUILD)/$(TARGET)

$(BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD)
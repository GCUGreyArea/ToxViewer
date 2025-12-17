

TARGET  = test_$(NAME)

PWD = $(shell pwd)

BUILD   = $(PWD)/build

CXXSRC = $(wildcard src/*.cpp)
CSRC   = $(wildcard src/*.c)

LIB     = lib$(NAME).a

CXXOBJ    := $(patsubst %.cpp,$(BUILD)/%.o,$(CXXSRC))
COBJ      := $(patsubst %.c,$(BUILD)/%.o,$(CSRC))

BUILD     = $(PWD)/build

LIBRARIES := -lgtest -lpthread
UNAME     := $(shell uname -s)

LNK = -Wl,-rpath,$(LIB)/build

all: $(TARGET) $(OBJ) $(COBJ) $(LIBDEP)

$(TARGET) : build $(CXXOBJ) $(COBJ)
	@echo "LIBDIR: $(LIBDIR)"
	@echo "LNK_PATH: $(LNK_PATH)"
	@echo "LIBRARIES: $(LIBRARIES)"

	$(CXX) $(CXXFLAGS) -fsanitize=address $(CXXOBJ) $(COBJ) -o $(BUILD)/$(TARGET) -L$(LIBDIR)/build -l$(NAME) $(LIBRARIES) -Wl,-rpath,$(LIBDIR)/$(BUILD)

build:
	mkdir -p "$(BUILD)/src"

$(LIBDEP) :
	cd $(LIBDIR) && make

$(BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
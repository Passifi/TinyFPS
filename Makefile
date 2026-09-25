# Compiler

CXX := g++
CC  := gcc

# Directories

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

# Output

BIN := tinyFPS

# Source files

CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)

# Object files

CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))
C_OBJS   := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

OBJS := $(CPP_OBJS) $(C_OBJS)

# ---------------------------------------------------------
# Platform-specific configuration
# ---------------------------------------------------------

ifeq ($(OS),Windows_NT)

    # Windows
    #
    # Change this if GLFW is installed somewhere else.
    GLFW_DIR := C:\Libs\glfw-3.5.1.bin.WIN64

    GLFW_INC := $(GLFW_DIR)/include
    GLFW_LIB := $(GLFW_DIR)/lib-mingw-w64

    CXXFLAGS := -std=c++17 -Wall -Wextra -g \
                -I$(INC_DIR) \
                -I$(GLFW_INC)

    CFLAGS := -Wall -Wextra \
              -I$(INC_DIR) \
              -I$(GLFW_INC)

    LDFLAGS := -L$(GLFW_LIB) \
               -lglfw3 \
               -lopengl32 \
               -lgdi32

    BIN := tinyFPS.exe

else

    # Linux

    CXXFLAGS := -std=c++17 -Wall -Wextra -g \
                -I$(INC_DIR) \
                $(shell pkg-config --cflags glfw3)

    CFLAGS := -Wall -Wextra \
              -I$(INC_DIR)

    LDFLAGS := $(shell pkg-config --libs glfw3) \
               -lGL \
               -ldl

endif

# ---------------------------------------------------------
# Default target
# ---------------------------------------------------------

all: $(BIN)

# ---------------------------------------------------------
# Link
# ---------------------------------------------------------

$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# ---------------------------------------------------------
# Compile C++
# ---------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------------------------------------------------------
# Compile C
# ---------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CC) $(CFLAGS) -c $< -o $@

# ---------------------------------------------------------
# Clean
# ---------------------------------------------------------

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN)

.PHONY: all clean
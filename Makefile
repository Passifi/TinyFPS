# Compiler
CXX      := g++
CC       := gcc

# Directories
SRC_DIR  := src
INC_DIR  := include
OBJ_DIR  := obj
BIN      := tinyFPS

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)

# Object files
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))
C_OBJS   := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

OBJS := $(CPP_OBJS) $(C_OBJS)

# Flags
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(INC_DIR) $(shell pkg-config --cflags glfw3)
CFLAGS   := -Wall -Wextra -I$(INC_DIR)
LDFLAGS  := $(shell pkg-config --libs glfw3) -lGL -ldl

# Default target
all: $(BIN)

# Link
$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile C++
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN)

.PHONY: all clean
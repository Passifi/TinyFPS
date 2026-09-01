# Compiler
CXX      := g++
CC       := gcc

# Directories
SRC_DIR  := src
INC_DIR  := include
BIN      := tinyFPS

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)

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
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C (for glad.c)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(SRC_DIR)/*.o $(BIN)

.PHONY: all clean

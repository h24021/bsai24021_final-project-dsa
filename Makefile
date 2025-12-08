# Library Management System - Makefile
# Author: Generated for DSA Final Project
# Date: December 2024

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g
INCLUDE_DIR = backend/include
SRC_DIR = backend/src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/library_system

# Source files
MODEL_SRCS = $(SRC_DIR)/models/Book.cpp $(SRC_DIR)/models/User.cpp
SERVICE_SRCS = $(SRC_DIR)/services/Library.cpp
CONTROLLER_SRCS = $(SRC_DIR)/controllers/BookController.cpp $(SRC_DIR)/controllers/UserController.cpp
API_SRCS = $(SRC_DIR)/api/Router.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp

# All source files
SRCS = $(MODEL_SRCS) $(SERVICE_SRCS) $(CONTROLLER_SRCS) $(MAIN_SRC)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include paths
INCLUDES = -I$(INCLUDE_DIR)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f main
	rm -rf main.dSYM
	@echo "Cleaned build artifacts"

# Build and run
build-run: all run

# Create necessary directories
setup:
	@mkdir -p $(BUILD_DIR)/models
	@mkdir -p $(BUILD_DIR)/services
	@mkdir -p $(BUILD_DIR)/controllers
	@mkdir -p $(BUILD_DIR)/api
	@echo "Build directories created"

# Help target
help:
	@echo "Library Management System - Makefile"
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  run        - Run the executable"
	@echo "  build-run  - Build and run the project"
	@echo "  clean      - Remove build artifacts"
	@echo "  setup      - Create build directories"
	@echo "  help       - Show this help message"

.PHONY: all run clean build-run setup help

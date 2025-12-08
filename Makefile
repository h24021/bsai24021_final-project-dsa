# Library Management System - Makefile
# Author: Generated for DSA Final Project
# Date: December 2024

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDE_DIR = backend/include
SRC_DIR = backend/src
BUILD_DIR = build
TEST_DIR = tests
TARGET = $(BUILD_DIR)/library_system
TEST_TARGET = $(BUILD_DIR)/test_btree

# Source files
MODEL_SRCS = $(SRC_DIR)/models/Book.cpp $(SRC_DIR)/models/User.cpp
SERVICE_SRCS = $(SRC_DIR)/services/Library.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp

# Test source files
TEST_SRCS = $(TEST_DIR)/test_btree.cpp

# All source files (without main for tests)
LIB_SRCS = $(MODEL_SRCS) $(SERVICE_SRCS)

# Object files
MAIN_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/main.o
TEST_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/test_btree.o

# Include paths
INCLUDES = -I$(INCLUDE_DIR)

# Default target
all: $(TARGET)

# Build and run tests
test: $(TEST_TARGET)
	@echo "\n========== Running Tests ==========\n"
	./$(TEST_TARGET)

# Link the main executable
$(TARGET): $(MAIN_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJS) -o $(TARGET)
	@echo "Build successful! Executable: $(TARGET)"

# Link the test executable
$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)
	@echo "Test build successful! Executable: $(TEST_TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile test files to object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Build everything (main + tests)
build-all: $(TARGET) $(TEST_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f main

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the main library system"
	@echo "  test       - Build and run tests"
	@echo "  run        - Build and run the main program"
	@echo "  build-all  - Build both main program and tests"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"

.PHONY: all test run build-all clean help
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

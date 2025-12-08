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
API_TARGET = $(BUILD_DIR)/api_server
TEST_TARGET = $(BUILD_DIR)/test_btree

# Source files
MODEL_SRCS = $(SRC_DIR)/models/Book.cpp $(SRC_DIR)/models/User.cpp
SERVICE_SRCS = $(SRC_DIR)/services/Library.cpp
HTTP_SRCS = $(SRC_DIR)/http/HttpModels.cpp
API_SRCS = $(SRC_DIR)/api/Router.cpp
CONTROLLER_SRCS = $(SRC_DIR)/controllers/BookControllerNew.cpp \
                  $(SRC_DIR)/controllers/UserControllerNew.cpp \
                  $(SRC_DIR)/controllers/BorrowController.cpp \
                  $(SRC_DIR)/controllers/StatisticsController.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp
API_MAIN_SRC = $(SRC_DIR)/main_api.cpp

# Test source files
TEST_SRCS = $(TEST_DIR)/test_btree.cpp

# All library source files (without main)
LIB_SRCS = $(MODEL_SRCS) $(SERVICE_SRCS)
API_LIB_SRCS = $(MODEL_SRCS) $(SERVICE_SRCS) $(HTTP_SRCS) $(API_SRCS) $(CONTROLLER_SRCS)

# Object files
MAIN_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/main.o
API_OBJS = $(API_LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/main_api.o
TEST_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/test_btree.o

# Include paths
INCLUDES = -I$(INCLUDE_DIR)

# Default target
all: $(TARGET) $(API_TARGET)

# Build API server
api: $(API_TARGET)
	@echo "\n========== API Server Built ==========\n"
	./$(API_TARGET)

# Build and run tests
test: $(TEST_TARGET)
	@echo "\n========== Running Tests ==========\n"
	./$(TEST_TARGET)

# Link the main executable
$(TARGET): $(MAIN_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJS) -o $(TARGET)
	@echo "Build successful! Executable: $(TARGET)"

# Link the API server executable
$(API_TARGET): $(API_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(API_OBJS) -o $(API_TARGET)
	@echo "API server build successful! Executable: $(API_TARGET)"

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

# Build everything (main + API + tests)
build-all: $(TARGET) $(API_TARGET) $(TEST_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f main
	rm -rf main.dSYM
	@echo "Cleaned build artifacts"

# Build and run main
build-run: all run

# Create necessary directories
setup:
	@mkdir -p $(BUILD_DIR)/models
	@mkdir -p $(BUILD_DIR)/services
	@mkdir -p $(BUILD_DIR)/controllers
	@mkdir -p $(BUILD_DIR)/api
	@mkdir -p $(BUILD_DIR)/http
	@echo "Build directories created"
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


CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDE_DIR = backend/include
SRC_DIR = backend/src
BUILD_DIR = build
TEST_DIR = tests
NET_API_TARGET = $(BUILD_DIR)/http_api_server
TEST_TARGET = $(BUILD_DIR)/test_btree


MODEL_SRCS = $(SRC_DIR)/models/Book.cpp $(SRC_DIR)/models/User.cpp
SERVICE_SRCS = $(SRC_DIR)/services/Library.cpp $(SRC_DIR)/services/SessionManager.cpp
HTTP_SRCS = $(SRC_DIR)/http/HttpModels.cpp $(SRC_DIR)/http/HttpServer.cpp
API_SRCS = $(SRC_DIR)/api/Router.cpp
CONTROLLER_SRCS = $(SRC_DIR)/controllers/BookControllerNew.cpp \
                  $(SRC_DIR)/controllers/UserControllerNew.cpp \
                  $(SRC_DIR)/controllers/BorrowController.cpp \
                  $(SRC_DIR)/controllers/StatisticsController.cpp \
                  $(SRC_DIR)/controllers/AuthController.cpp
NET_API_MAIN_SRC = $(SRC_DIR)/main_http.cpp

TEST_SRCS = $(TEST_DIR)/test_btree.cpp

LIB_SRCS = $(MODEL_SRCS) $(SERVICE_SRCS)
API_LIB_SRCS = $(MODEL_SRCS) $(SERVICE_SRCS) $(HTTP_SRCS) $(API_SRCS) $(CONTROLLER_SRCS)

NET_API_OBJS = $(API_LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/main_http.o
TEST_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/test_btree.o

INCLUDES = -I$(INCLUDE_DIR)

all: $(NET_API_TARGET)

network-api: $(NET_API_TARGET)
	@echo "\n========== Networked HTTP API Server Built ==========\n"
	./$(NET_API_TARGET)

test: $(TEST_TARGET)
	@echo "\n========== Running Tests ==========\n"
	./$(TEST_TARGET)

$(NET_API_TARGET): $(NET_API_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(NET_API_OBJS) -o $(NET_API_TARGET)
	@echo "HTTP API server build successful! Executable: $(NET_API_TARGET)"


$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)
	@echo "Test build successful! Executable: $(TEST_TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(NET_API_TARGET)
	./$(NET_API_TARGET)

build-all: $(NET_API_TARGET) $(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f main
	rm -rf main.dSYM
	@echo "Cleaned build artifacts"

build-run: all run

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
	@echo "  all         - Build HTTP API server (default)"
	@echo "  run         - Build and run the HTTP server"
	@echo "  network-api - Build the HTTP API server (listen on :8080)"
	@echo "  test        - Build and run tests"
	@echo "  build-all   - Build HTTP server and tests"
	@echo "  clean       - Remove build artifacts"
	@echo "  setup       - Create build directories"
	@echo "  help        - Show this help message"

.PHONY: all run clean build-all setup help network-api test
